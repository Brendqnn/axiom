#include "ebo.h"


EBO ebo_create()
{
    EBO ebo;
    glGenBuffers(1, &ebo.ID);
    return ebo;
}

void ebo_destroy(EBO ebo)
{
    glDeleteBuffers(1, &ebo.ID);
}

void ebo_bind(EBO ebo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.ID);
}

void ebo_unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_buffer(EBO ebo, unsigned int indices[], size_t size)
{
    ebo_bind(ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
