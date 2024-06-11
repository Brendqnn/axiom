#include "axvao.h"


AXVao ax_create_vao()
{
    AXVao vao;
    glGenVertexArrays(1, &vao.ID);
    return vao;
}

void ax_destroy_vao(AXVao vao)
{
    glDeleteVertexArrays(1, &vao.ID);
}

void ax_bind_vao(AXVao vao)
{
    glBindVertexArray(vao.ID);
}

void ax_link_attrib(AXVao vao, AXVbo vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset)
{
    ax_bind_vbo(vbo);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (GLvoid*)offset);
    glEnableVertexAttribArray(index);
    ax_bind_vbo((AXVbo){0});
}


