#ifndef AXVAO_H
#define AXVAO_H

#include <GL/glew.h>
#include "axvbo.h"

typedef struct {
    GLuint ID;
} AXVao;

AXVao ax_create_vao();
void ax_destroy_vao(AXVao vao);
void ax_bind_vao(AXVao vao);
void ax_link_attrib(AXVao vao, AXVbo vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);

#endif // AXVAO_H
