#include "vbo.h"

VBO vbo_create(GLint type, bool dynamic)
{
    VBO vbo = {
        .type = type,
        .dynamic = dynamic
    };
    
    glGenBuffers(1, &vbo.ID);
        
    return vbo;
}

void vbo_destroy(VBO vbo)
{
    glDeleteBuffers(1, &vbo.ID);
}

void vbo_bind(VBO vbo)
{
    glBindBuffer(vbo.type, vbo.ID);
}

void vbo_buffer(VBO vbo, float vertices[], size_t size)
{
    vbo_bind(vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

