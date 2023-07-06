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

#include "camera.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1080

const char* vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * view * vec4(aPos, 1.0);\n"
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

    Camera camera;
    camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 90.0f);

    glfwSwapInterval(1); // Enable VSync

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disabled cursor

    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    glEnable(GL_DEPTH_TEST);

    double previous_time = glfwGetTime();
    double frame_time = 1.0 / 60.0;

    // Set the user pointer to pass the Camera pointer to the callback
    glfwSetWindowUserPointer(window, &camera);

    // Set the cursor position callback
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Center the cursor initially
    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;
    glfwSetCursorPos(window, center_x, center_y);

    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        // Process input
        camera_update(&camera, window, delta_time);

        // Calculate the view matrix
        mat4 view;
        camera_get_view_matrix(&camera, view);

        // Use the shader program
        glUseProgram(shader.ID);

        // Bind the VAO
        vao_bind(vao);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

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


