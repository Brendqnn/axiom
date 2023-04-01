#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "vao.h"
#include "vbo.h"



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800


const char* fragment_shader =
        "#version 330 core\n"
        "in vec3 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(color, 1.0f);\n"
        "}\n";

const char* vertex_shader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    color = aColor;\n"
        "}\n";


int main(void) {
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Triangle", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

	
    GLuint vbo, color_vbo;
	setup_vbo(vbo, color_vbo);

	GLuint vao;
	setup_vao(vao);
    
    Shader shader = shader_create(vertex_shader, fragment_shader);
   
		
    while (!glfwWindowShouldClose(window)) {
   
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.ID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
	shader_destroy(&shader);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &color_vbo);
    glDeleteVertexArrays(1, &vao);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}


