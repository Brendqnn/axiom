#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"

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
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "    color = aColor;\n"
    "}\n";

int main(void) {
    GLFWwindow* window;

    printf("im tired xD");
    
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "sup", NULL, NULL);
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
	
    GLuint vao;
    setup_vao(vao);

    Shader shader = shader_create(vertex_shader, fragment_shader);

    // Initialize matrices
    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_perspective(glm_rad(25.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    glm_translate(view, (vec3){0.0f, 0.0f, -5.0f});

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the model matrix to rotate the pyramid
	glm_rotate(model, glfwGetTime() * 0.25f, (vec3){0.0f, 1.0f, 0.0f});

	// Set the model, view, and projection matrices in the shader program
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

	// Draw the pyramid
	glUseProgram(shader.ID);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);

	// Swap buffers and poll events
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

