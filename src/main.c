#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "shader.h"
#include "model.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void calculate_fps(double frame_time) {
    static double previous_time = 0.0;
    static int frame_count = 0;

    double current_time = glfwGetTime();
    double elapsed_time = current_time - previous_time;
    frame_count++;
    if (elapsed_time >= 1.0) {
        double fps = frame_count / elapsed_time;
        printf("FPS: %.00f\r", fps);
        frame_count = 0;
        previous_time = current_time;
        fflush(stdout);
    }
}

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
    camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 60.0f);

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1); // Enable VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor
    
    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    
    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    Model* tree = load_model("res/maple/source/tree2.blend");
            
    double previous_time = glfwGetTime();
    double frame_time = 1.0 / 60.0;
    
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;
    glfwSetCursorPos(window, center_x, center_y);
         
    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        camera_update(&camera, window, delta_time);

        camera_get_view_matrix(&camera, view);
        glUseProgram(shader.ID);

        draw_model(tree);

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            shader_destroy(&shader);
            exit(1);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    shader_destroy(&shader);
    model_destroy(tree);
    glfwTerminate();
    return 0;
}















