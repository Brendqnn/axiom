#include <stdio.h>
#include <stdlib.h>

#include <cglm/cglm.h>
#include <noise1234.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <time.h>

#include "util/camera.h"
#include "gfx/window.h"
#include "util/util.h"
#include "models/model.h"
#include "gfx/shader.h"
#include "gfx/skybox.h"
#include "util/rtm.h"

#define TERRAIN_HEIGHT 100
#define TERRAIN_WIDTH 100
#define TERRAIN_SCALE 100.0f
#define PEAK_SCALE 2.0f

void gen_terrain(float *vertices)
{
    float xstart = -TERRAIN_HEIGHT/2.0f * TERRAIN_SCALE;
    float zstart = -TERRAIN_WIDTH/2.0f * TERRAIN_SCALE;

    int index = 0;
    for (int x = 0; x < TERRAIN_HEIGHT; ++x) {
        for (int z = 0; z < TERRAIN_WIDTH; ++z) {
            // Triangle 1
            vertices[index++] = xstart + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2(x * 0.1f, z * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + z * TERRAIN_SCALE;

            vertices[index++] = xstart + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2((x + 1) * 0.1f, z * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + z * TERRAIN_SCALE;

            vertices[index++] = xstart + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2(x * 0.1f, (z + 1) * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + (z + 1) * TERRAIN_SCALE;

            // Triangle 2
            vertices[index++] = xstart + x * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2(x * 0.1f, (z + 1) * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + (z + 1) * TERRAIN_SCALE;

            vertices[index++] = xstart + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2((x + 1) * 0.1f, z * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + z * TERRAIN_SCALE;

            vertices[index++] = xstart + (x + 1) * TERRAIN_SCALE;
            vertices[index++] = PEAK_SCALE * noise2((x + 1) * 0.1f, (z + 1) * 0.1f) * TERRAIN_SCALE;
            vertices[index++] = zstart + (z + 1) * TERRAIN_SCALE;
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
    ImGui_ImplOpenGL3_Init("#version 460");// Enable docking
    ImGui::StyleColorsDark();

    bool my_tool_active = true;

    const char* skybox_face_paths[6] = { 
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg"
    };

    int num_vertices = TERRAIN_WIDTH * TERRAIN_HEIGHT * 6 * 3;
    float terrain[num_vertices];
    gen_terrain(terrain);

    VAO terrain_vao = vao_create();
    VBO terrain_vbo = vbo_create(GL_ARRAY_BUFFER, false);

    vao_bind(terrain_vao);
    vbo_buffer(terrain_vbo, terrain, sizeof(terrain));
    link_attrib(terrain_vao, terrain_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    Model model_t = load_model("res/Bench.obj");

    Shader shader = shader_create("res/shaders/default.vert", "res/shaders/default.frag");
    Shader test = shader_create("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 100.0f};
    vec3 up = {0.0f, 100.0f, 0.0f};
    
    camera_init(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);

    mat4 terrain_model_mat;
 
    set_cursor_pos_callback(&camera, &axiom);

    while (!ax_window_should_close(&axiom)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_update(&camera, &axiom);

        double memory = get_memory_usage_mb();

        glm_mat4_identity(model_t.matrix);

        draw_model(model_t, &shader);
        shader_uniform_mat4(&shader, "view", camera.view);
        shader_uniform_mat4(&shader, "projection", camera.projection);
        shader_uniform_mat4(&shader, "model", model_t.matrix);

        glm_mat4_identity(terrain_model_mat);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader_use(&test);
        shader_uniform_mat4(&test, "view", camera.view);
        shader_uniform_mat4(&test, "projection", camera.projection);
        shader_uniform_mat4(&test, "model", terrain_model_mat);

        vao_bind(terrain_vao);
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        skybox_render(skybox, &camera); 

        if (glfwGetKey(axiom.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(axiom.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            free_model(&model_t);
            exit(1);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Performance");
        double fps = calculate_fps(axiom.last_frame);
        ImGui::Text("FPS: %.00f\r", fps);
        ImGui::Text("Memory: %.02f MB", memory);

        // Add more elements here
        ImGui::Separator();
        ImGui::Text("Additional Information:");
        //ImGui::Checkbox("Wireframe Mode", &wireframe_mode);
        //ImGui::SliderFloat("Terrain Scale", &terrain_scale, 1.0f, 100.0f);
        //ImGui::ColorEdit3("Background Color", background_color);
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
