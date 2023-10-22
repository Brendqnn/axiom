#include "shader.h"
#include <stdio.h>
#include <stdlib.h>


static void check_compile_errors(GLuint shader, char *type)
{
    GLint success;
    GLchar infoLog[1024];
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

char* read_shader_source(const char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = (char*)malloc(length + 1);
    if (!source) {
        fprintf(stderr, "Failed to allocate memory for shader source.\n");
        fclose(file);
        return NULL;
    }

    fread(source, 1, length, file);
    source[length] = '\0';

    //printf("Shader source:\n%s\n", source); // Add this line for debug output

    fclose(file);
    return source;
}

Shader shader_create(const char* vertex_shader_path, const char* fragment_shader_path)
{
    Shader self;

    const char* vs_src = read_shader_source(vertex_shader_path);
    const char* fs_src = read_shader_source(fragment_shader_path);

    if (!vs_src || !fs_src) {
        if (vs_src) free((void*)vs_src);
        if (fs_src) free((void*)fs_src);
        Shader empty_shader = { 0 };
        return empty_shader;
    }

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_src, NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, "VERTEX");

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
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

void shader_destroy(Shader *shader)
{
    glDeleteProgram(shader->ID);
    shader->ID = 0;
}
