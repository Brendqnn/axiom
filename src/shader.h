#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

typedef struct Shader {
    unsigned int ID;
} Shader;

Shader shader_create(const char* vs_src, const char* fs_src);
char* read_shader_source(const char* file_path);
void shader_setint(const Shader *shader, const char* name, int value);
void shader_use(const Shader *shader);
void shader_destroy(Shader *shader);

#endif
