#ifndef IBO_H
#define IBO_H

#include <GL/glew.h>

struct IBO {
    GLuint id;
};

struct IBO ibo_create();
void ibo_destroy(struct IBO ibo);
void ibo_bind(struct IBO ibo);
void ibo_unbind();

void ibo_buffer(struct IBO ibo, const void* data, GLsizeiptr size, GLenum usage);

#endif /* IBO_H */
