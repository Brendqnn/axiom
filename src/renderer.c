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

    GLfloat vertices[] =
	{
		-0.5f, -0.5f * sqrt(3) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * sqrt(3) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * sqrt(3) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * sqrt(3) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * sqrt(3) / 6, 0.0f, // Inner right
		0.0f, -0.5f * sqrt(3) / 3, 0.0f // Inner down
	};

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
    };

    renderer->vao = vao_create();
    renderer->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    renderer->ibo = ibo_create();

    vao_bind(renderer->vao);

    vbo_buffer(renderer->vbo, vertices, 0, sizeof(vertices));
    vao_attr(renderer->vao, renderer->vbo, 0, 3, GL_FLOAT, 0, 0);

    ibo_buffer(renderer->ibo, indices, sizeof(indices), GL_STATIC_DRAW);
    renderer->index_count = sizeof(indices) / sizeof(GLuint);
}

void renderer_render(Renderer* renderer, GLFWwindow* window) {
    static double previous_time = 0.0;
    double current_time = glfwGetTime();
    float delta_time = current_time - previous_time;
    previous_time = current_time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_update(&renderer->camera, window, delta_time);

    camera_get_view_matrix(&renderer->camera, renderer->view);

    glUseProgram(renderer->shader.ID);

    // Set the model, view, and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "model"), 1, GL_FALSE, (float*)renderer->model);
    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "view"), 1, GL_FALSE, (float*)renderer->view);
    glUniformMatrix4fv(glGetUniformLocation(renderer->shader.ID, "projection"), 1, GL_FALSE, (float*)renderer->projection);

    // Bind the VAO with the IBO (since we're using an IBO to specify the vertex indices)
    vao_bind(renderer->vao);
    ibo_bind(renderer->ibo);

    // Draw the triangles using the IBO (by specifying the number of indices in the index array)
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);
}

void renderer_destroy(Renderer *renderer) {
    vao_destroy(renderer->vao);
    vbo_destroy(renderer->vbo);
    shader_destroy(&renderer->shader);
    texture_unbind(&renderer->texture);
    texture_cleanup(&renderer->texture);
}
