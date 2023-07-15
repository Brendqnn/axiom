#include "ibo.h"

struct IBO ibo_create() {
    struct IBO ibo;
    glGenBuffers(1, &ibo.id);
    return ibo;
}

void ibo_destroy(struct IBO ibo) {
    glDeleteBuffers(1, &ibo.id);
}

void ibo_bind(struct IBO ibo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
}

void ibo_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ibo_buffer(struct IBO ibo, const void* data, GLsizeiptr size, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
