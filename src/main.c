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
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * view *vec4(aPos, 1.0);\n"
	"    color = aColor;\n"
	"}\n";


int main(void) {
    GLFWwindow* window;

    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "yo", NULL, NULL);
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

    GLuint vbo, color_vbo;
	setup_vbo(vbo, color_vbo);

	GLuint vao;setup_vao(vao);

    Shader shader = shader_create(vertex_shader, fragment_shader);

    // Define camera parameters
    vec3 camera_pos = {0.0f, 0.0f, 3.0f};
    vec3 camera_target = {0.0f, 0.0f, 0.0f};
    vec3 camera_up = {0.0f, 1.0f, 0.0f};

    // Calculate view matrix using camera parameters
    mat4 view_matrix;
    glm_lookat(camera_pos, camera_target, camera_up, view_matrix);

    // Set up camera uniform in shader
    GLuint view_location = glGetUniformLocation(shader.ID, "view_matrix");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (float*)view_matrix);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.ID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(&shader);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &color_vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}


