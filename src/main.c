#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "vbo.h"
#include "vao.h"
#include "camera.h"
#include "texture.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

const char* vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n" // Add this line for texture coordinates
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "out vec2 TexCoord;\n" 
    "void main()\n"
    "{\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    TexCoord = aTexCoord;\n" 
    "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\n";

const char* fragment_shader =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec2 TexCoord;\n" 
    "out vec4 FragColor;\n"
    "uniform sampler2D textureSampler;\n" 
    "void main()\n"
    "{\n"
    "    vec3 lightDir = normalize(vec3(0.0, 1.0, 0.0));\n"
    "    float diff = max(dot(normalize(Normal), lightDir), 0.0);\n"
    "    vec3 diffuse = diff * texture(textureSampler, TexCoord).rgb;\n" 
    "    FragColor = vec4(diffuse, 1.0);\n"
    "}\n";

void calculate_fps(double frame_time) {
    static double previous_time = 0.0;
    static int frame_count = 0;

    double current_time = glfwGetTime();
    double elapsed_time = current_time - previous_time;
    frame_count++;
    if (elapsed_time >= frame_time) {
        double fps = frame_count / elapsed_time;
        printf("FPS: %.00f\r", fps);  
        frame_count = 0;
        previous_time = current_time;
        fflush(stdout);  
    }
}

void processMesh(const struct aiMesh* mesh, const struct aiScene* scene, struct VAO vao, struct VBO vbo) {
    // Process vertex positions
    int vertex_count = mesh->mNumVertices;
    int vertices_size = 3 * vertex_count * sizeof(float);
    float* vertices = (float*)malloc(vertices_size);

    for (unsigned int i = 0; i < vertex_count; i++) {
        struct aiVector3D pos = mesh->mVertices[i];
        vertices[i * 3] = pos.x;
        vertices[i * 3 + 1] = pos.y;
        vertices[i * 3 + 2] = pos.z;
    }
    
    vbo_bind(vbo);
    vbo_buffer(vbo, vertices, vertices_size, GL_STATIC_DRAW);
    
    vao_bind(vao);
    vao_attr(vao, vbo, 0, 3, GL_FLOAT, 0, 0);

    free(vertices);
}

void processNode(const struct aiNode* node, const struct aiScene* scene, struct VAO* vao, struct VBO* vbo) {
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, *vao, *vbo);
    }

    // Process all the children nodes (recursively)
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, vao, vbo);
    }
}

void loadModel(const char* path, struct VAO* vao, struct VBO* vbo, int* num_vertices) {
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Failed to load model: %s\n", aiGetErrorString());
        return;
    }

    *num_vertices = 0;
    processNode(scene->mRootNode, scene, vao, vbo);

    // Calculate the total number of vertices
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        *num_vertices += scene->mMeshes[i]->mNumVertices;
    }
    printf("%d\n", *num_vertices);
    aiReleaseImport(scene);
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

    struct VBO vbo = vbo_create(GL_ARRAY_BUFFER, false);
    struct VAO vao = vao_create();

    vbo_bind(vbo);
    vao_bind(vao);

    Shader shader = shader_create(vertex_shader, fragment_shader);

    Camera camera;
    camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 60.0f);

    glfwSwapInterval(1); // Enable VSync

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_perspective(glm_rad(camera.fov), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

    glEnable(GL_DEPTH_TEST);

    double previous_time = glfwGetTime();
    double frame_time = 1.0 / 60.0;

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Center the cursor initially
    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;
    glfwSetCursorPos(window, center_x, center_y);

    int num_vertices = 0;
    loadModel("res/cherry/source/cherry.blend", &vao, &vbo, &num_vertices);

    Texture texture = texture_load("res/cherry/textures/cherry_Color.png", true);
    if (texture.id == 0) {
        // Handle texture loading error
        printf("Failed to load texture\n");
    }
 
    while (!glfwWindowShouldClose(window)) {
        calculate_fps(frame_time);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        double current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        // Process input
        camera_update(&camera, window, delta_time);

        mat4 view;
        camera_get_view_matrix(&camera, view);

        glUseProgram(shader.ID);
        vao_bind(vao);

        glm_mat4_identity(model);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, (float*)projection);

        texture_bind(&texture, 0);

        glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            shader_destroy(&shader);
            vao_destroy(vao);
            vbo_destroy(vbo);
            exit(1);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(&shader);
    vao_destroy(vao);
    vbo_destroy(vbo);

    glfwTerminate();
    return 0;
}


















