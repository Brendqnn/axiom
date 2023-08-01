#ifndef EBO_H
#define EBO_H

#include <GL/glew.h>

struct EBO {
    GLuint id;
};

struct EBO ebo_create();
void ebo_destroy(struct EBO ebo);
void ebo_bind(struct EBO ebo);
void ebo_unbind();

void ebo_buffer(struct EBO ebo, const void* data, GLsizeiptr size, GLenum usage);

#endif /* EBO_H */
