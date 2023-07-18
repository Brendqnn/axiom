#include "renderer.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void renderer_init(Renderer* renderer, const char* vertex_shader_src, const char* fragment_shader_src, GLFWwindow* window) {
    camera_init(&renderer->camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 60.0f);
    renderer->shader = shader_create(vertex_shader_src, fragment_shader_src);

    glm_mat4_identity(renderer->model);
    glm_mat4_identity(renderer->view);
    glm_mat4_identity(renderer->projection);

    glm_perspective(glm_rad(renderer->camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, renderer->projection);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &renderer->camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;
    glfwSetCursorPos(window, center_x, center_y);

    glfwSwapInterval(1); // Enable VSync
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // Positions
        -0.5f, -0.5f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.0f,  // Bottom-right
        0.5f, 0.5f, 0.0f,   // Top-right

        0.5f, 0.5f, 0.0f,   // Top-right
        -0.5f, 0.5f, 0.0f,  // Top-left
        -0.5f, -0.5f, 0.0f   // Bottom-left
    };

   
    renderer->vao = vao_create();
    renderer->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    //renderer->ibo = ibo_create();

    vao_bind(renderer->vao);

    vbo_buffer(renderer->vbo, vertices, 0, sizeof(vertices));
    vao_attr(renderer->vao, renderer->vbo, 0, 3, GL_FLOAT, 0, 0);
   
    renderer->vertex_count = 6; // 6 vertices for two triangles (square)
    renderer->index_count = 0;  // 6 indices for two triangles (square)
}


void renderer_render(Renderer* renderer, GLFWwindow *window) {
    static double previous_time = 0.0;
    double current_time = glfwGetTime();
    float delta_time = current_time - previous_time;
    previous_time = current_time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_update(&renderer->camera, window, delta_time);

    camera_get_view_matrix(&renderer->camera, renderer->view);

    glUseProgram(renderer->shader.ID);
    vao_bind(renderer->vao);

    //glm_mat4_identity(renderer->model);

    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "model"), 1, GL_FALSE, (float*)renderer->model);
    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "view"), 1, GL_FALSE, (float*)renderer->view);
    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "projection"), 1, GL_FALSE, (float*)renderer->projection);

    //texture_bind(&renderer->texture, 0);

    glDrawArrays(GL_TRIANGLES, 0, renderer->vertex_count);
}

void renderer_destroy(Renderer *renderer) {
    vao_destroy(renderer->vao);
    vbo_destroy(renderer->vbo);
    shader_destroy(&renderer->shader);
    texture_unbind(&renderer->texture);
    texture_cleanup(&renderer->texture);
}

