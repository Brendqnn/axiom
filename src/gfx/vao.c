#include "vao.h"

void vao_create(VAO *vao) {
    glGenVertexArrays(1, &(vao->id));
}

void vao_bind(const VAO *vao) {
    glBindVertexArray(vao->id);
}

void vao_unbind() {
    glBindVertexArray(0);
}

void vao_linkattrib(const VAO *vao, const VBO *vbo, GLuint index, size_t size, GLenum type, GLsizei stride, const void* offset) {
    vbo_bind(vbo);
    vao_bind(vao);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
}
