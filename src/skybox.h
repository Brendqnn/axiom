#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "texture.h"

typedef struct {
    float skybox_vertices[24];
    unsigned int skybox_vertices[36];
    unsigned int VAO, VBO, EBO;    
} Skybox;

void init_skybox();

#endif // SKYBOX_H
