#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "gfx/shader.h"

#include "camera.h"

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

float vertices[] = {
    // Front face
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,

    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

float colors[] = {
    // Front face
    0.0f, 0.0f, 0.75f, 
    0.0f, 0.0f, 0.75f,      
    0.0f, 0.0f, 0.75f,     

    0.0f, 0.0f, 0.75f,     
    0.0f, 0.0f, 0.75f,    
    0.0f, 0.0f, 0.75f       
};

void setup_vbo(GLuint* vbo, GLuint* color_vbo) {
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void setup_vao(GLuint* vao, GLuint vbo, GLuint color_vbo) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

double get_current_time() {
    return glfwGetTime();
}

void calculate_fps(double frame_time) {
    static double previous_time = 0.0;
    static int frame_count = 0;

    double current_time = get_current_time();
    double elapsed_time = current_time - previous_time;
    frame_count++;
    if (elapsed_time >= frame_time) {
        double fps = frame_count / elapsed_time;
        printf("FPS: %.0f\n", fps);

        frame_count = 0;
        previous_time = current_time;
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

    GLuint vbo, color_vbo;
    setup_vbo(&vbo, &color_vbo);

    GLuint vao;
    setup_vao(&vao, vbo, color_vbo);

    float square_size = 1.0f;

    // Calculate the number of squares in each row and column
    int num_squares = 3;
    int num_vertices = 6 * num_squares * num_squares;

    // Calculate the total size of the vertex and color arrays
    int vertices_size = 3 * num_vertices * sizeof(float);
    int colors_size = 3 * num_vertices * sizeof(float);

    // Allocate memory for the vertex and color arrays
    float* vertices = (float*)malloc(vertices_size);
    float* colors = (float*)malloc(colors_size);

    // Fill the vertex and color arrays with positions and colors for each square
    int vertex_index = 0;
    int color_index = 0;

    for (int row = 0; row < num_squares; row++) {
        for (int col = 0; col < num_squares; col++) {
            // Calculate the position of the current square
            float x = col * square_size;
            float y = 0.0f; // Set the y-coordinate as needed for the floor
            float z = row * square_size;

            // Define the vertices for the current square
            float square_vertices[] = {
                x, y, z,
                x + square_size, y, z,
                x + square_size, y, z + square_size,

                x, y, z,
                x + square_size, y, z + square_size,
                x, y, z + square_size
            };

            // Define the color for the current square
            float square_color[] = {
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,

                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f
            };

            // Copy the vertices and colors for the current square to the arrays
            memcpy(vertices + vertex_index, square_vertices, sizeof(square_vertices));
            memcpy(colors + color_index, square_color, sizeof(square_color));

            // Update the indices for the next square
            vertex_index += sizeof(square_vertices) / sizeof(float);
            color_index += sizeof(square_color) / sizeof(float);
        }
    }

    Shader shader = shader_create(vertex_shader, fragment_shader);

    Camera camera;
    camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 45.0f);

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

    float outline_scale = 0.95f; // Adjust this value to control the thickness of the outline

    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, colors_size, colors, GL_STATIC_DRAW);

        camera_update(&camera, window, delta_time);

        camera_get_view_matrix(&camera, view);

        glUseProgram(shader.ID);
        glBindVertexArray(vao);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

        // Enable wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int numSquares = num_squares * num_squares; // Number of squares in the grid
        for (int i = 0; i < numSquares; i++) {
            glDrawArrays(GL_TRIANGLES, i * 6, 6);
        }

        // Disable wireframe mode and switch back to filling mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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








