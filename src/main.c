#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "texture.h"

void calculate_fps()
{
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

float skybox_vertices[] = {
    -1.0f, -1.0f,  1.0f,//        7--------6
    1.0f, -1.0f,  1.0f,//       /|       /|
    1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
    1.0f,  1.0f,  1.0f,//      |/       |/
    1.0f,  1.0f, -1.0f,//      0--------1
    -1.0f,  1.0f, -1.0f
};

unsigned int skybox_indices[] = {
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
};

void remove_translation_from_matrix(mat4 input_matrix, mat4 output_matrix)
{
    // Extract upper-left 3x3 submatrix
    mat3 view_without_translation_mat3;
    glm_mat4_pick3(input_matrix, view_without_translation_mat3);

    // Create a 4x4 matrix with the extracted 3x3 matrix
    glm_mat4_identity(output_matrix);
    glm_mat4_ins3(view_without_translation_mat3, output_matrix);
}

int main(void)
{
    if (!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW.\n");
        glfwTerminate();
        return -1;
    }

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indices), &skybox_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    const char* skybox_face_paths[6] = {
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    };

    Texture cubemap = load_cubemap_texture(skybox_face_paths);

    //Model model_t = load_model("assets/backpack/backpack.obj");
    Model model_t = load_model("assets/tree/Prunus_Pendula.obj");
    
    Shader shader = shader_create("src/gfx/default.vert", "src/gfx/default.frag");
    Shader skybox = shader_create("src/gfx/skybox.vert", "src/gfx/skybox.frag");

    Camera camera;
    camera_init(&camera, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);
    
    glfwSwapInterval(1); // Enable VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    double previous_time = glfwGetTime();
    
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    double center_x = WINDOW_WIDTH/2;
    double center_y = WINDOW_HEIGHT/2;
    
    glfwSetCursorPos(window, center_x, center_y);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        camera_update(&camera, window, delta_time);
        
        glm_mat4_identity(view);
        camera_get_view_matrix(&camera, view);

        // Render Model
        shader_use(&shader);
        glm_mat4_identity(model);
        glm_scale(model, (vec3){0.1f, 0.1f, 0.1f});

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

        draw_model(model_t, shader);

        mat4 view_without_translation;
        remove_translation_from_matrix(view, view_without_translation);

        glDepthFunc(GL_LEQUAL);
        shader_use(&skybox);

        glUniformMatrix4fv(glGetUniformLocation(skybox.ID, "view"), 1, GL_FALSE, (float*)view_without_translation);
        glUniformMatrix4fv(glGetUniformLocation(skybox.ID, "projection"), 1, GL_FALSE, (float*)projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
       
        calculate_fps();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
