#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>
#include "vbo.h"

typedef struct {
    GLuint ID;
} VAO;

/**
 * Create a Vertex Array Object (VAO).
 *
 * @return A VAO structure representing the created VAO.
 */
VAO vao_create();

/**
 * Destroy the VAO.
 *
 * @param vao The VAO structure to be destroyed.
 */
void vao_destroy(VAO vao);

/**
 * Bind the VAO for rendering.
 *
 * @param vao The VAO structure to be bound.
 */
void vao_bind(VAO vao);

/**
 * Link an attribute to a VBO in the VAO.
 *
 * @param vao    The VAO structure.
 * @param vbo    The VBO structure.
 * @param index  The attribute index.
 * @param size   The number of components in the attribute.
 * @param type   The data type of the attribute.
 * @param stride The stride between consecutive attributes.
 * @param offset The offset of the attribute data.
 */
void link_attrib(VAO vao, VBO vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);

#endif // VAO_H
