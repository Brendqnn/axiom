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

#define TERRAIN_SIZE_X 50
#define TERRAIN_SIZE_Z 50
#define TERRAIN_SCALE 0.1f

void gen_terrain(float vertices[])
{
    float xstart = -TERRAIN_SIZE_X / 2.0f;
    float zstart = -TERRAIN_SIZE_Z / 2.0f;

    for (int x = 0; x < TERRAIN_SIZE_X; ++x) {
        for (int z = 0; z < TERRAIN_SIZE_Z; ++z) {
            float height = noise2(x * 0.1f, z * 0.1f);
            vertices[(x * TERRAIN_SIZE_Z + z) * 3] = xstart + x * TERRAIN_SCALE;
            vertices[(x * TERRAIN_SIZE_Z + z) * 3 + 1] = height;
            vertices[(x * TERRAIN_SIZE_Z + z) * 3 + 2] = zstart + z * TERRAIN_SCALE;
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
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom", NULL, NULL);
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

    int num_vertices = (TERRAIN_SIZE_X + 1) * (TERRAIN_SIZE_Z + 1);

    float terrain_vertices[num_vertices * 3];

    gen_terrain(terrain_vertices);

    GLuint terrainVAO, terrainVBO;
    glGenVertexArrays(1, &terrainVAO);
    glGenBuffers(1, &terrainVBO);
    glBindVertexArray(terrainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_vertices), terrain_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
    Shader test = shader_create("res/shaders/test.vert", "res/shaders/test.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    camera_init(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);
         
    //glfwSwapInterval(1); // Enable VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, camera.projection);
    double previous_time = glfwGetTime();
    
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;
        
        camera_update(&camera, window, delta_time);
        camera_get_view_matrix(&camera);

        glm_mat4_identity(camera.model);

        vec3 translation_vector = {0.0f, 0.0f, -2.0f}; // Adjust the value as needed
        glm_translate(camera.model, translation_vector);

        vec3 scale_model = {0.5f, 0.5f, 0.5f};
        glm_scale(camera.model, scale_model);

        draw_model(model_t, &shader);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)camera.view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)camera.projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)camera.model);

        shader_use(&test);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "view"), 1, GL_FALSE, (float*)camera.view);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "projection"), 1, GL_FALSE, (float*)camera.projection);
        glUniformMatrix4fv(glGetUniformLocation(test.ID, "model"), 1, GL_FALSE, (float*)camera.model);

        glBindVertexArray(terrainVAO);
        glDrawArrays(GL_POINTS, 0, num_vertices);
        glBindVertexArray(0);

        skybox_render(skybox, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
