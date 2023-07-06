#include "vbo.h"  


void vbo_create(VBO *vbo, const void *data, GLsizei data_size, GLenum usage) {
    glGenBuffers(1, &(vbo->id));
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
    glBufferData(GL_ARRAY_BUFFER, data_size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_bind(const VBO *vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
}

void vbo_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
