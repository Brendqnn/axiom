#ifndef SKYBOX_H
#define SKYBOX_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shader.h"
#include "texture.h"
#include "../util/camera.h"


typedef struct {
    VAO vao;
    VBO vbo;
    EBO ebo;

    Texture cubemap;
    Shader shader;
} Skybox;

Skybox skybox_init(const char *skybox_faces[]);
void skybox_render(Skybox skybox, Camera *camera);

#endif // SKYBOX_H
