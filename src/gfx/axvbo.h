#ifndef AXVBO_H
#define AXVBO_H

#include <GL/glew.h>
#include <stdbool.h>

typedef struct {
    GLuint ID;
    GLint type;
    bool dynamic;
} AXVbo;

AXVbo ax_create_vbo(GLint type, bool dynamic);
void ax_destroy_vbo(AXVbo vbo);
void ax_bind_vbo(AXVbo vbo);
void ax_bind_vbo_buffer(AXVbo vbo, float vertices[], size_t size);

#endif // AXVBO_H
