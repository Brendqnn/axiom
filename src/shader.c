#include "shader.h"
#include <stdio.h>
#include <stdlib.h>


static void check_compile_errors(GLuint shader, char *type) {
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

char* read_shader_source(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("Failed to open shader file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        printf("Failed to allocate memory for shader source.\n");
        fclose(file);
        return NULL;
    }

    // Check if all data was read correctly
    size_t bytes_read = fread(buffer, 1, length, file);
    if (bytes_read != (size_t)length) {
        printf("Failed to read shader source from file: %s\n", file_path);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

Shader shader_create(const char* vertex_shader_path, const char* fragment_shader_path) {
    Shader self;

    // Read shader sources from file
    const char* vs_src = read_shader_source(vertex_shader_path);
    const char* fs_src = read_shader_source(fragment_shader_path);

    if (!vs_src || !fs_src) {
        // Handle error, e.g., display an error message and exit the program.
        if (vs_src) free((void*)vs_src);
        if (fs_src) free((void*)fs_src);
        Shader empty_shader = { 0 };
        return empty_shader;
    }

    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_src, NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, "VERTEX");

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_src, NULL);
    glCompileShader(fragment_shader);
    check_compile_errors(fragment_shader, "FRAGMENT");

    // shader program
    self.ID = glCreateProgram();
    glAttachShader(self.ID, vertex_shader);
    glAttachShader(self.ID, fragment_shader);
    glLinkProgram(self.ID);
    check_compile_errors(self.ID, "PROGRAM");

    // Free the allocated shader sources after creating the shader
    free((void*)vs_src);
    free((void*)fs_src);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return self;
}

void shader_destroy(Shader *shader) {
    glDeleteProgram(shader->ID);
    shader->ID = 0;
}
