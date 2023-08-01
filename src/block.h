#ifndef BLOCK_H
#define BLOCK_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"


typedef struct Block {
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;

    float block_size;
    float vertices[24];
    unsigned int indices[36];
} Block;

void setup_block(Block *block);
void draw_block(Block *block);

#endif //BLOCK_H
