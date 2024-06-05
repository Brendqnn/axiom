#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "util/camera.h"
#include "gfx/window.h"
#include "util/util.h"
#include "models/model.h"
#include "gfx/shader.h"
#include "gfx/skybox.h"
#include "util/rtm.h"


bool is_camera_near_edge(Camera *camera)
{
    float terrian_size = TERRAIN_SCALE*TERRAIN_SCALE/2.0f;
    float abs_x = fabs(camera->position[0]);
    float abs_y = fabs(camera->position[2]);

    if (abs_x + camera->view_distance > terrian_size || abs_y + camera->view_distance > terrian_size) {
        return true;
    }

    return false;
}

static inline float get_terrain_frequency(float min, float max)
{
    return min + ((float)rand()/RAND_MAX) * (max - min); 
}

void gen_terrain(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT], float vertices[])
{
    float x_start = -TERRAIN_WIDTH / 2.0f * TERRAIN_SCALE;
    float z_start = -TERRAIN_HEIGHT / 2.0f * TERRAIN_SCALE;

    int index = 0;
    float peak_scale = 1.0f;

    for (int x = 0; x < TERRAIN_WIDTH; ++x) {
        for (int z = 0; z < TERRAIN_HEIGHT; ++z) {
            vertices[index++] = x_start + x * TERRAIN_SCALE;
            vertices[index++] = peak_scale * map[x][z] * TERRAIN_SCALE;
            vertices[index++] = z_start + z * TERRAIN_SCALE;
        }
    }
}

void gen_texture_coordinates(float texture_coords[], float scale_factor)
{
    int index = 0;
    for (int x = 0; x < TERRAIN_WIDTH; ++x) {
        for (int z = 0; z < TERRAIN_HEIGHT; ++z) {
            float s = (float)x / TERRAIN_WIDTH * scale_factor;
            float t = (float)z / TERRAIN_HEIGHT * scale_factor;
            
            texture_coords[index++] = s;
            texture_coords[index++] = t;
        }
    }
}

void gen_terrain_indices(unsigned int indices[]) {
    int index = 0;
    for (int x = 0; x < TERRAIN_WIDTH - 1; ++x) {
        for (int z = 0; z < TERRAIN_HEIGHT - 1; ++z) {
            int topLeft = x * TERRAIN_HEIGHT + z;
            int topRight = topLeft + 1;
            int bottomLeft = (x + 1) * TERRAIN_HEIGHT + z;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices[index++] = topLeft;
            indices[index++] = bottomLeft;
            indices[index++] = topRight;

            // Second triangle
            indices[index++] = topRight;
            indices[index++] = bottomLeft;
            indices[index++] = bottomRight;
        }
    }
}

void generate_heightmap(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT])
{
    for (int x = 0; x < TERRAIN_WIDTH + 1; x++) { 
        for (int z = 0; z < TERRAIN_HEIGHT + 1; z++) {
            float height = pnoise3(x * 0.1f, 1.0f, z * 0.1f, 1024, 1024, 1024);
            map[x][z] = height;
        }
    }
}

void print_heightmap(float heightmap[TERRAIN_WIDTH][TERRAIN_HEIGHT]) 
{
    for (int x = 0; x < TERRAIN_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_HEIGHT; z++) {
            printf("%.2f\n", heightmap[x][z]);
        }
    }
}

int main(void)
{
    Window axiom = ax_window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v1.0");
    axiom.fullscreen = true;
    toggle_fullscreen(&axiom);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(axiom.handle, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();

    bool vsync = true;

    const char* skybox_face_paths[6] = {
        "res/skybox/px.jpg",
        "res/skybox/nx.jpg",
        "res/skybox/py.jpg",
        "res/skybox/ny.jpg",
        "res/skybox/pz.jpg",
        "res/skybox/nz.jpg"
    };

    int num_vertices = TERRAIN_WIDTH * TERRAIN_HEIGHT * 3;
    int num_indices = (TERRAIN_WIDTH - 1) * (TERRAIN_HEIGHT - 1) * 6;
    float map[TERRAIN_WIDTH][TERRAIN_HEIGHT];
    float vertices[num_vertices];
    float texture_coords[TERRAIN_WIDTH * TERRAIN_HEIGHT * 2];
    unsigned int indices[num_indices];

    Texture dirt = load_texture("res/terrain/forest_ground_04_diff_4k.jpg");
    Texture dirt_grass = load_texture("res/terrain/forrest_ground_01_diff_4k.jpg");

    generate_heightmap(map);
    gen_terrain(map, vertices);
    float texture_scale = 62.0f;
    gen_texture_coordinates(texture_coords, texture_scale);
    gen_terrain_indices(indices);

    VAO terrain_vao = vao_create();
    VBO terrain_vbo = vbo_create(GL_ARRAY_BUFFER, false);
    VBO terrain_tbo = vbo_create(GL_ARRAY_BUFFER, false);
    EBO terrain_ebo = ebo_create();

    vao_bind(terrain_vao);

    vbo_buffer(terrain_vbo, vertices, sizeof(vertices));
    link_attrib(terrain_vao, terrain_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    vbo_buffer(terrain_tbo, texture_coords, sizeof(texture_coords));
    link_attrib(terrain_vao, terrain_tbo, 1, 2, GL_FLOAT, 2 * sizeof(float), 0);

    ebo_buffer(terrain_ebo, indices, sizeof(indices));

    Model model_t = load_model("res/models/trees/Prunus_Pendula.obj");

    Shader shader = shader_create("res/shaders/default.vert", "res/shaders/default.frag");
    Shader test = shader_create("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 100.0f};
    vec3 up = {0.0f, 50.0f, 0.0f};

    camera_init(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);

    set_cursor_pos_callback(&camera, &axiom);

    mat4 terrain_model_mat;

    while (!ax_window_should_close(&axiom)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_update(&camera, &axiom);

        double memory = get_memory_usage_mb();

        glm_mat4_identity(model_t.matrix);

        vec3 scale_vector = {10.0f, 10.0f, 10.0f};
        glm_scale(model_t.matrix, scale_vector);

        draw_model(model_t, &shader);
        shader_uniform_mat4(&shader, "view", camera.view);
        shader_uniform_mat4(&shader, "projection", camera.projection);
        shader_uniform_mat4(&shader, "model", model_t.matrix);

        glm_mat4_identity(terrain_model_mat);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirt.id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dirt_grass.id);

        shader_use(&test);
        shader_setint(&test, "terrainTextures[0]", 0);
        shader_setint(&test, "terrainTextures[1]", 1);
        
        shader_uniform_mat4(&test, "view", camera.view);
        shader_uniform_mat4(&test, "projection", camera.projection);
        shader_uniform_mat4(&test, "model", terrain_model_mat);

        vao_bind(terrain_vao);
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        skybox_render(skybox, &camera); 

        if (glfwGetKey(axiom.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(axiom.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            free_model(&model_t);
            destroy_texture(&dirt);
            destroy_texture(&dirt_grass);
            exit(1);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_viewport->ID, main_viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Performance");
        double fps = calculate_fps();
        ImGui::Text("FPS: %.00f\r", fps);
        ImGui::Separator();
        ImGui::Text("Memory: %.02f MB", memory);

        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Text("Enable V-Sync");
        if (ImGui::Checkbox(" ", &vsync)) {
            if (vsync) {
                enable_vsync();
            } else {
                disable_vsync();
            }
        }
        ImGui::Separator();
        ImGui::Text("Render Distance");
        if (ImGui::SliderFloat(" ", &(camera.view_distance), 1000.0f, 20000.0f)) {
            set_camera_view(&camera);
        }
        ImGui::End();

        ImGui::Begin("Camera");
        ImGui::Text("Camera Position: (%.02f, %.02f, %.02f)\n", camera.position[0], camera.position[1], camera.position[2]);
        bool edges = is_camera_near_edge(&camera);
        ImGui::Checkbox(" ", &edges);
        ImGui::Separator();
        ImGui::Text("Movement Speed");
        ImGui::SliderFloat(" ", &(camera.movement_speed), 100.0f, 1000.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (glfwGetKey(axiom.handle, GLFW_KEY_R) == GLFW_PRESS) {
            camera.debug_ui = true;
            disable_cursor_capture(axiom.handle);
        }

        if (glfwGetKey(axiom.handle, GLFW_KEY_T) == GLFW_PRESS) {
            camera.debug_ui = false;
            enable_cursor_capture(&camera, &axiom);
        }

        ImGui::UpdatePlatformWindows();

        glfwSwapBuffers(axiom.handle);
        glfwPollEvents();
    }

    glfwDestroyWindow(axiom.handle);
    glfwTerminate();
    return 0; 
}
