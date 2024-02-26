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
    for (int x = 0; x < TERRAIN_WIDTH; ++x) {
        for (int z = 0; z < TERRAIN_HEIGHT; ++z) {
            // Vertex 1
            vertices[index++] = x_start + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x][z] * TERRAIN_SCALE;
            vertices[index++] = z_start + z * TERRAIN_SCALE;

            // Vertex 2
            vertices[index++] = x_start + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x + 1][z] * TERRAIN_SCALE;
            vertices[index++] = z_start + z * TERRAIN_SCALE;

            // Vertex 3
            vertices[index++] = x_start + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x][z + 1] * TERRAIN_SCALE;
            vertices[index++] = z_start + (z + 1) * TERRAIN_SCALE;

            // Vertex 4
            vertices[index++] = x_start + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x][z + 1] * TERRAIN_SCALE;
            vertices[index++] = z_start + (z + 1) * TERRAIN_SCALE;

            // Vertex 5
            vertices[index++] = x_start + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x + 1][z] * TERRAIN_SCALE;
            vertices[index++] = z_start + z * TERRAIN_SCALE;

            // Vertex 6
            vertices[index++] = x_start + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * map[x + 1][z + 1] * TERRAIN_SCALE;
            vertices[index++] = z_start + (z + 1) * TERRAIN_SCALE;
        }
    }
}

void generate_heightmap(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT])
{
    for (int x = 0; x < TERRAIN_WIDTH + 1; x++) { 
        for (int z = 0; z < TERRAIN_HEIGHT + 1; z++) {
            float frequency = get_terrain_frequency(1.0f, 0.01f);
            float height = pnoise3(x * 0.1f, frequency, z * 0.1f, 1024, 1024, 1024);
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
    Window axiom = ax_window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v1.11");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(axiom.handle, true);
    ImGui_ImplOpenGL3_Init("#version 460"); // Enable docking
    ImGui::StyleColorsDark();

    bool my_tool_active = true;
    bool vsync = false;

    const char* skybox_face_paths[6] = { 
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg"
    };

    int num_vertices = TERRAIN_WIDTH * TERRAIN_HEIGHT * 6 * 3;
    float terrain[num_vertices]; // Each vertex has 3 coordinates
    float map[TERRAIN_WIDTH][TERRAIN_HEIGHT];
    generate_heightmap(map);

    gen_terrain(map, terrain);

    vec2 offsets[2] = {
        {0.0f, 0.0f},   // Offset for the first instance
        {10000.0f, 0.0f}  // Offset for the second instance
    };

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 2, &offsets[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    GLuint terrain_vao, terrain_vbo;
    glGenVertexArrays(1, &terrain_vao);
    glGenBuffers(1, &terrain_vbo);

    glBindVertexArray(terrain_vao);

    glBindBuffer(GL_ARRAY_BUFFER, terrain_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(float), terrain, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);    
    glVertexAttribDivisor(2, 1);

    //Model model_t = load_model("res/tree/scene.gltf");

    //Shader shader = shader_create("res/shaders/default.vert", "res/shaders/default.frag");
    Shader test = shader_create("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 100.0f};
    vec3 up = {0.0f, 100.0f, 0.0f};
    
    camera_init(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);

    set_cursor_pos_callback(&camera, &axiom);

    while (!ax_window_should_close(&axiom)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_update(&camera, &axiom);

        double memory = get_memory_usage_mb();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader_use(&test);
        shader_uniform_mat4(&test, "view", camera.view);
        shader_uniform_mat4(&test, "projection", camera.projection);

        for (int i = 0; i < 2; ++i) {
            char uniformName[20];
            sprintf(uniformName, "offsets[%d]", i); 

            shader_uniform_mat4(&test, uniformName, offsets[i]);
        }

        glBindVertexArray(terrain_vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, num_vertices, 2);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        skybox_render(skybox, &camera); 

        if (glfwGetKey(axiom.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(axiom.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            exit(1);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_viewport, ImGuiDockNodeFlags_PassthruCentralNode);

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
        if (ImGui::SliderFloat(" ", &(camera.view_distance), 1000.0f, 10000.0f)) {
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
