#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "renderer.h"

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
   

    Renderer renderer;
    renderer_init(&renderer, "default.vert", "default.frag", window);
    
    double previous_time = glfwGetTime();
    double frame_time = 1.0 / 60.0;
    
    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);
        
        renderer_render(&renderer, window);

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame_time = glfwGetTime() - previous_time;
    }

    renderer_destroy(&renderer);
    glfwTerminate();
    return 0;
}

















