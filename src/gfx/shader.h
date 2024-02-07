#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
//#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>


typedef struct Shader {
    unsigned int ID;
} Shader;

/**
 * Create a shader program from vertex and fragment shader source code.
 *
 * @param vs_src The source code of the vertex shader.
 * @param fs_src The source code of the fragment shader.
 *
 * @return A Shader structure representing the created shader program.
 */
Shader shader_create(const char* vs_src, const char* fs_src);

/**
 * Read the source code from a shader file.
 *
 * @param file_path The path to the shader file.
 *
 * @return A dynamically allocated string containing the shader source code.
 */
char* read_shader_source(const char* file_path);

/**
 * Set an integer value in the shader.
 *
 * @param shader The Shader structure.
 * @param name   The name of the uniform variable.
 * @param value  The integer value to set.
 */
void shader_setint(const Shader *shader, const char* name, int value);

/**
 * Use the shader program for rendering.
 *
 * @param shader The Shader structure.
 */
void shader_use(const Shader *shader);

/**
 * Set uniform shader.
 *
 * @param shader The Shader structure
 * @param name Name of the source matrix being passed
 * @param matrix The matrix for the source shader
 */
void shader_uniform_mat4(Shader *shader, const char* name, void *matrix);

/**
 * Destroy the shader program.
 *
 * @param shader The Shader structure.
 */
void shader_destroy(Shader *shader);

#endif // SHADER_H
