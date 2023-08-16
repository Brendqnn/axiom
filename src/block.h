#ifndef BLOCK_H
#define BLOCK_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"

typedef struct Block {
    float vertices[24];
    unsigned int indices[36];
    
    float block_size;
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
} Block;

Block *create_block(float block_size);
void destroy_block(Block *block);
void draw_block(Block *block);

#endif // BLOCK_H



