#include "axvbo.h"

AXVbo ax_create_vbo(GLint type, bool dynamic)
{
    AXVbo vbo = {
        .type = type,
        .dynamic = dynamic
    };
    
    glGenBuffers(1, &vbo.ID);
        
    return vbo;
}

void ax_destroy_vbo(AXVbo vbo)
{
    glDeleteBuffers(1, &vbo.ID);
}

void ax_bind_vbo(AXVbo vbo)
{
    glBindBuffer(vbo.type, vbo.ID);
}

void ax_bind_vbo_buffer(AXVbo vbo, float vertices[], size_t size)
{
    ax_bind_vbo(vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

