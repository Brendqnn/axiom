#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vbo.h"

typedef struct {
    GLuint id;
} VAO;

void vao_create(VAO *vao);
void vao_bind(const VAO *vao);
void vao_unbind();
void vao_linkattrib(const VAO *vao, const VBO *vbo, GLuint index, size_t size, GLenum type, GLsizei stride, const void* offset);

#endif // VAO_H
