#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <string.h>


typedef struct {
    unsigned int ID;
} AXShader;

AXShader ax_create_shader(const char* vs_src, const char* fs_src);
char* ax_read_shader(const char* file_path);
void ax_set_shader_int(const AXShader *shader, const char* name, int value);
void ax_use_shader(const AXShader *shader);
void ax_uniform_shader_mat4(AXShader *shader, const char* name, void *matrix);
void ax_free_shader(AXShader *shader);

#endif // SHADER_H
