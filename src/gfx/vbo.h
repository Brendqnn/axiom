#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <stdbool.h>

typedef struct {
    GLuint ID;
    GLint type;
    bool dynamic;
} VBO;

/**
 * Create a Vertex Buffer Object (VBO).
 *
 * @param type    The type of the buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
 * @param dynamic Indicates whether the buffer is dynamic or static.
 *
 * @return A VBO structure representing the created VBO.
 */
VBO vbo_create(GLint type, bool dynamic);

/**
 * Destroy the VBO.
 *
 * @param vbo The VBO structure to be destroyed.
 */
void vbo_destroy(VBO vbo);

/**
 * Bind the VBO for rendering.
 *
 * @param vbo The VBO structure to be bound.
 */
void vbo_bind(VBO vbo);

/**
 * Buffer data into the VBO.
 *
 * @param vbo      The VBO structure.
 * @param vertices The array of vertices to be buffered.
 * @param size     The size of the data to be buffered.
 */
void vbo_buffer(VBO vbo, float vertices[], size_t size);

#endif // VBO_H
