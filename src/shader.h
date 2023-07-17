#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

typedef struct Shader {
    GLuint ID;
} Shader;

Shader shader_create(const char* vs_src, const char* fs_src);

void shader_destroy(Shader *shader);

#endif
