#ifndef AXEBO_H
#define AXEBO_H

#include <GL/glew.h>

typedef struct {
    GLuint ID;
} AXEbo;

AXEbo ax_create_ebo();
void ax_free_ebo(AXEbo ebo);
void ax_bind_ebo(AXEbo ebo);
void ax_bind_ebo_buffer(AXEbo ebo, unsigned int indices[], size_t size);

#endif // EBO_H
