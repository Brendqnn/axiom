#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "shader.h"
#include "util/util.h"
#include "block.h"


int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }
    
    Shader shader = shader_create("default.vert", "default.frag");

    Camera camera;
    camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, CAMERA_FOV, 10);

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // Enable VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    double previous_time = glfwGetTime();
    
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;
    
    glfwSetCursorPos(window, center_x, center_y);

    Block *block = create_block();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;
        calculate_fps(delta_time);
        // Process input
        camera_update(&camera, window, delta_time);

        camera_get_view_matrix(&camera, view);
        glUseProgram(shader.ID);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

        glUniform1i(glGetUniformLocation(shader.ID, "textureSampler"), 0);

        draw_block(block);

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            shader_destroy(&shader);
            
            exit(1);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shader_destroy(&shader);
    glfwTerminate();
    return 0;
}



