#include "ebo.h"

struct EBO ebo_create() {
    struct EBO ebo;
    glGenBuffers(1, &ebo.id);
    return ebo;
}

void ebo_destroy(struct EBO ebo) {
    glDeleteBuffers(1, &ebo.id);
}

void ebo_bind(struct EBO ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
}

void ebo_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_buffer(struct EBO ebo, const void* data, GLsizeiptr size, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
