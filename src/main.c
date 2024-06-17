#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <slcodec.h>
#include <pthread.h>
#include <windows.h>

#include "util/axcamera.h"
#include "gfx/axwindow.h"
#include "util/axutil.h"
#include "models/axmodel.h"
#include "gfx/axshader.h"
#include "gfx/axskybox.h"
#include "util/axrtm.h"

/*
  
   ▒███▒      ▒██      ██▒   ▒██████▒   ▄██████▄    ▒███       ███▒      
  ███ ███      ███    ███      ▀██▀    ███    ███    ████     ████
 ███   ███      ███  ███        ██     ███    ███    ██ ██▄ ▄██ ██
███     ███      ▒████▒         ██     ▒██    ██▒    ██  ▀███▀  ██
▒█████████▒     ███  ███        ██     ███    ███    ██    ▀    ██
███     ███    ███    ███      ▄██▄    ███    ███    ██         ██
███     ███   ▒██      ██▒   ▒██████░   ▀██████▀   ▄████▄     ▄████▄

*/


void *play_audio(void *args)
{
    SLAudioDevice *audio_device = ((SLAudioDevice**)args)[0];
    SLStreamContext *stream_ctx = ((SLStreamContext**)args)[1];

    Sleep(30000);
    sl_play_audio_buffer(audio_device, stream_ctx);

    return NULL;
}

bool is_camera_near_edge(AXCamera *camera)
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

void gen_terrain_indices(unsigned int indices[])
{
    int index = 0;
    for (int x = 0; x < TERRAIN_WIDTH - 1; ++x) {
        for (int z = 0; z < TERRAIN_HEIGHT - 1; ++z) {
            int top_left = x * TERRAIN_HEIGHT + z;
            int top_right = top_left + 1;
            int bottom_left = (x + 1) * TERRAIN_HEIGHT + z;
            int bottom_right = bottom_left + 1;

            indices[index++] = top_left;
            indices[index++] = bottom_left;
            indices[index++] = top_right;

            indices[index++] = top_right;
            indices[index++] = bottom_left;
            indices[index++] = bottom_right;
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

/* void generate_heightmap(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT]) */
/* { */
/*     int pad_width = TERRAIN_WIDTH / 8;  // Padding size for smoother transition */
/*     int pad_height = TERRAIN_HEIGHT / 8;  // Padding size for smoother transition */
/*     float min_height = -50.0f;  // Minimum height threshold to prevent negative peaks */

/*     for (int x = 0; x < TERRAIN_WIDTH + 1; x++) { */
/*         for (int z = 0; z < TERRAIN_HEIGHT + 1; z++) { */
/*             float height = pnoise3(x * 0.1f, 0.1f, z * 0.1f, 1024, 1024, 1024); */

/*             if (x > 2 * TERRAIN_WIDTH / 4 - pad_width && z > 2 * TERRAIN_HEIGHT / 4 - pad_height) { */
/*                 float scale_factor = 1.0f; */

/*                 if (x > 2 * TERRAIN_WIDTH / 4 && z > 2 * TERRAIN_HEIGHT / 4) { */
/*                     scale_factor = 5.0f;  // Maximum height in the core of the region */
/*                 } else { */
/*                     float x_dist = (x - (2 * TERRAIN_WIDTH / 4 - pad_width)) / (float)pad_width; */
/*                     float z_dist = (z - (2 * TERRAIN_HEIGHT / 4 - pad_height)) / (float)pad_height; */
/*                     scale_factor = 1.0f + (4.0f - 1.0f) * fminf(x_dist, z_dist); */
/*                 } */

/*                 height *= scale_factor; */
/*             } */

/*             // Apply minimum height threshold */
/*             if (height < min_height) { */
/*                 height = min_height; */
/*             } */

/*             map[x][z] = height; */
/*         } */
/*     } */
/* } */

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
    AXWindow window  = ax_window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v1.0");
    window.fullscreen = true;
    ax_toggle_fullscreen(&window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
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

    AXTexture dirt = ax_load_texture("res/terrain/forest_ground_04_diff_4k.jpg");
    AXTexture dirt_grass = ax_load_texture("res/terrain/forrest_ground_01_diff_4k.jpg");

    generate_heightmap(map);
    gen_terrain(map, vertices);
    
    float texture_scale = TERRAIN_SCALE;
    
    gen_texture_coordinates(texture_coords, texture_scale);
    gen_terrain_indices(indices);

    AXVao terrain_vao = ax_create_vao();
    AXVbo terrain_vbo = ax_create_vbo(GL_ARRAY_BUFFER, false);
    AXVbo terrain_tbo = ax_create_vbo(GL_ARRAY_BUFFER, false);
    AXEbo terrain_ebo = ax_create_ebo();

    ax_bind_vao(terrain_vao);

    ax_bind_vbo_buffer(terrain_vbo, vertices, sizeof(vertices));
    ax_link_attrib(terrain_vao, terrain_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    ax_bind_vbo_buffer(terrain_tbo, texture_coords, sizeof(texture_coords));
    ax_link_attrib(terrain_vao, terrain_tbo, 1, 2, GL_FLOAT, 2 * sizeof(float), 0);

    ax_bind_ebo_buffer(terrain_ebo, indices, sizeof(indices));

    /* Model model_t = load_model("res/models/scene.gltf"); */

    AXShader shader = ax_create_shader("res/shaders/default.vert", "res/shaders/default.frag");
    AXShader test = ax_create_shader("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    AXSkybox skybox = ax_init_skybox(skybox_face_paths);

    AXCamera camera;
    vec3 position = {0.0f, 0.0f, 100.0f};
    vec3 up = {0.0f, 50.0f, 0.0f};

    ax_init_camera(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);

    ax_set_cursor_pos_callback(&camera, &window);

    mat4 terrain_model_mat;

    SLAudioDevice audio_device = {0};
    SLStreamContext stream_ctx = {0};
    
    sl_open_input_file("res/sfx/cicadas.mp3", &stream_ctx);
    
    sl_setup_resampler(&stream_ctx, SL_SAMPLE_RATE_96, 0.1f);
    sl_decode_resample(&stream_ctx);

    sl_set_audio_playback_device(&audio_device, &stream_ctx);

    pthread_t audio_thread;
    void *args[] = {&audio_device, &stream_ctx};

    if (pthread_create(&audio_thread, NULL, play_audio, (void *)args)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    while (!ax_window_should_close(&window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ax_update_camera(&camera, &window);

        double memory = get_memory_usage_mb();

        /* glm_mat4_identity(model_t.matrix); */

        /* vec3 scale_vector = {10.0f, 10.0f, 10.0f}; */
        /* glm_scale(model_t.matrix, scale_vector); */

        /* draw_model(model_t, &shader); */
        /* ax_uniform_shader_mat4(&shader, "view", camera.view); */
        /* ax_uniform_shader_mat4(&shader, "projection", camera.projection); */
        /* ax_uniform_shader_mat4(&shader, "model", model_t.matrix); */

        glm_mat4_identity(terrain_model_mat);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirt.id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dirt_grass.id);

        ax_use_shader(&test);
        ax_set_shader_int(&test, "terrainTextures[0]", 0);
        ax_set_shader_int(&test, "terrainTextures[1]", 1);
        
        ax_uniform_shader_mat4(&test, "view", camera.view);
        ax_uniform_shader_mat4(&test, "projection", camera.projection);
        ax_uniform_shader_mat4(&test, "model", terrain_model_mat);

        ax_bind_vao(terrain_vao);
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ax_render_skybox(skybox, &camera); 

        if (glfwGetKey(window.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            /* free_model(&model_t); */
            ax_destroy_texture(&dirt);
            ax_destroy_texture(&dirt_grass);
            ax_free_shader(&test);
            ax_free_shader(&shader);
            exit(1);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_viewport->ID, main_viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Performance");
        double fps = ax_calculate_fps();
        ImGui::Text("FPS: %.00f\r", fps);
        ImGui::Separator();
        ImGui::Text("Memory: %.02f MB", memory);

        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Text("Enable V-Sync");
        if (ImGui::Checkbox(" ", &vsync)) {
            if (vsync) {
                ax_enable_vsync();
            } else {
                ax_disable_vsync();
            }
        }
        ImGui::Separator();
        ImGui::Text("Render Distance");
        if (ImGui::SliderFloat(" ", &(camera.view_distance), 1000.0f, 20000.0f)) {
            ax_set_camera_view(&camera);
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

        if (glfwGetKey(window.handle, GLFW_KEY_R) == GLFW_PRESS) {
            camera.debug_ui = true;
            ax_disable_cursor_capture(window.handle);
        }

        if (glfwGetKey(window.handle, GLFW_KEY_T) == GLFW_PRESS) {
            camera.debug_ui = false;
            ax_enable_cursor_capture(&camera, &window);
        }

        ImGui::UpdatePlatformWindows();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    if (pthread_join(audio_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    ax_destroy_texture(&dirt);
    ax_destroy_texture(&dirt_grass);
    ax_free_shader(&test);
    ax_free_shader(&shader);
    
    glfwDestroyWindow(window.handle);
    glfwTerminate();
    
    return 0; 
}
