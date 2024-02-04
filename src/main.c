#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "util/camera.h"
#include "gfx/shader.h"
#include "models/model.h"
#include "gfx/skybox.h"
#include "util/util.h"
#include "gfx/noise1234.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

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
    if (!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v.1.11", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW.\n");
        glfwTerminate();
        return -1;
    }

    //Texture land = load_texture("res/land.jpg");

    int num_vertices = TERRAIN_WIDTH * TERRAIN_HEIGHT * 6 * 3;
    float terrain[num_vertices];
    gen_terrain(terrain);

    VAO terrain_vao = vao_create();
    VBO terrain_vbo = vbo_create(GL_ARRAY_BUFFER, false);
    
    vao_bind(terrain_vao);
    vbo_buffer(terrain_vbo, terrain, sizeof(terrain));
    link_attrib(terrain_vao, terrain_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

   const char* skybox_face_paths[6] = { 
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg"
    };

    Model model_t = load_model("res/tree/Prunus_Pendula.gltf");
    
    Shader shader = shader_create("res/shaders/default.vert", "res/shaders/default.frag");
    Shader test = shader_create("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 50.0f};
    vec3 up = {0.0f, 10.0f, 0.0f};
    
    camera_init(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);
         
    //glfwSwapInterval(1); // Enable VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 3000.0f, camera.projection);
    double previous_time = glfwGetTime();
    
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    mat4 model_model_mat, model_terrain_mat;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;
        
        camera_update(&camera, window, delta_time);
        camera_get_view_matrix(&camera);

        glm_mat4_identity(model_model_mat);

        vec3 translation_vector = {50.0f, 10.0f, 0.0f};
        glm_translate(model_model_mat, translation_vector);

        float angle = glm_rad(5.0f);
        vec3 axis = {0.0f, 0.0f, -2.0f};

        glm_rotate(model_model_mat, angle, axis);
        
        //vec3 scale_model = {0.1f, 0.1f, 0.1f};
        //glm_scale(camera.model, scale_model);

        draw_model(model_t, &shader);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)camera.view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)camera.projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model_model_mat);

        glm_mat4_identity(model_terrain_mat);
        
        shader_use(&test);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "view"), 1, GL_FALSE, (float*)camera.view);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "projection"), 1, GL_FALSE, (float*)camera.projection);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "model"), 1, GL_FALSE, (float*)model_terrain_mat);

        vao_bind(terrain_vao); 
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);
        glBindVertexArray(0);

        skybox_render(skybox, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
