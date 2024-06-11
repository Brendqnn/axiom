#include "axshader.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


static void check_compile_errors(unsigned int shader, const char *type)
{
    int success;
    char infoLog[1024];
    if (strcmp(type, "VERTEX") == 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
    else if (strcmp(type, "FRAGMENT") == 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
    else if (strcmp(type, "PROGRAM") == 0) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}

char* ax_read_shader(const char* file_path)
{
    FILE *f;
    char *text;
    long len;

    f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "Error loading shader at %s\n", file_path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    assert(len > 0);
    fseek(f, 0, SEEK_SET);

    text = (char*)calloc(1, len + 1);
    assert(text != NULL);
    fread(text, 1, len, f);
    assert(strlen(text) > 0);
    fclose(f);

    return text;
}

AXShader ax_create_shader(const char* vertex_shader_path, const char* fragment_shader_path)
{
    AXShader self;

    const char* vs_src = ax_read_shader(vertex_shader_path);
    const char* fs_src = ax_read_shader(fragment_shader_path);

    if (!vs_src || !fs_src) {
        if (vs_src) free((void*)vs_src);
        if (fs_src) free((void*)fs_src);
        AXShader empty_shader = {0};
        return empty_shader;
    }

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_src, NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, "VERTEX");

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_src, NULL);
    glCompileShader(fragment_shader);
    check_compile_errors(fragment_shader, "FRAGMENT");

    self.ID = glCreateProgram();
    glAttachShader(self.ID, vertex_shader);
    glAttachShader(self.ID, fragment_shader);
    glLinkProgram(self.ID);
    check_compile_errors(self.ID, "PROGRAM");

    free((void*)vs_src);
    free((void*)fs_src);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return self;
}

void ax_use_shader(const AXShader *shader)
{
    glUseProgram(shader->ID);
}

void ax_set_shader_int(const AXShader *shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void ax_uniform_shader_mat4(AXShader *shader, const char *name, void *matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (float*)matrix);
}

void ax_free_shader(AXShader *shader)
{
    glDeleteProgram(shader->ID);
    shader->ID = 0;
}
