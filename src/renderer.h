#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "vbo.h"
#include "vao.h"
#include "ibo.h"

typedef struct Renderer {
    Camera camera;
    Shader shader;
    struct VBO vbo;
    struct VAO vao;
    struct IBO ibo;
    int vertex_count;
    int index_count;
    Texture texture;
    mat4 model, view, projection;
} Renderer;

void renderer_init(Renderer* renderer, const char* vertex_shader_path, const char* fragment_shader_path, GLFWwindow *window);
void renderer_render(Renderer* renderer, GLFWwindow *window);
void renderer_destroy(Renderer* renderer);

#endif // RENDERER_H

