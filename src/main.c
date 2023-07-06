#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "gfx/shader.h"
#include "vbo.h"
#include "vao.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1080

const char* vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}\n";

const char* fragment_shader =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(0.3, 0.5, 0.8, 1.0);\n"
    "}\n";

void calculate_fps(double frame_time) {
    static double previous_time = 0.0;
    static int frame_count = 0;

    double current_time = glfwGetTime();
    double elapsed_time = current_time - previous_time;
    frame_count++;
    if (elapsed_time >= frame_time) {
        double fps = frame_count / elapsed_time;
        printf("FPS: %.0f\r", fps);  // Use \r to overwrite the current line
        
        frame_count = 0;
        previous_time = current_time;
        fflush(stdout);  // Flush the output to ensure it's displayed immediately
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

    // Create VBO and VAO
    struct VBO vbo = vbo_create(GL_ARRAY_BUFFER, false);
    struct VAO vao = vao_create();

    // Create vertex data
    float vertices[] = {
        // Front face
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,

        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Create and bind the VBO
    vbo_buffer(vbo, vertices, 0, sizeof(vertices));

    // Create the VAO
    vao_bind(vao);

    // Link the VBO and attribute pointers
    vao_attr(vao, vbo, 0, 3, GL_FLOAT, 0, 0);
    
    Shader shader = shader_create(vertex_shader, fragment_shader);

    glfwSwapInterval(1); // Enable VSync

    glEnable(GL_DEPTH_TEST);

    double previous_time = glfwGetTime();
    double frame_time = 1.0 / 60.0;

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);
        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        // Use the shader program
        glUseProgram(shader.ID);

        // Bind the VAO
        vao_bind(vao);

        // Draw the square
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(&shader);

    glfwTerminate();
    return 0;
}


