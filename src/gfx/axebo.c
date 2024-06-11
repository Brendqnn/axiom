#include "axebo.h"


AXEbo ax_create_ebo()
{
    AXEbo ebo;
    glGenBuffers(1, &ebo.ID);
    return ebo;
}

void ax_free_ebo(AXEbo ebo)
{
    glDeleteBuffers(1, &ebo.ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ax_bind_ebo(AXEbo ebo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.ID);
}

void ax_bind_ebo_buffer(AXEbo ebo, unsigned int indices[], size_t size)
{
    ax_bind_ebo(ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
