#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLuint id;
} VBO;

void vbo_create(VBO *vbo, const void *data, GLsizei data_size, GLenum usage);
void vbo_bind(const VBO *vbo);
void vbo_unbind();

#endif // VBO_H
