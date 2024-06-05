#include "vao.h"


VAO vao_create()
{
    VAO vao;
    glGenVertexArrays(1, &vao.ID);
    return vao;
}

void vao_destroy(VAO vao)
{
    glDeleteVertexArrays(1, &vao.ID);
}

void vao_bind(VAO vao)
{
    glBindVertexArray(vao.ID);
}

void link_attrib(VAO vao, VBO vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset)
{
    vbo_bind(vbo);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (GLvoid*)offset);
    glEnableVertexAttribArray(index);
    vbo_bind((VBO){0});
}


