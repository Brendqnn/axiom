#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

typedef struct Block {
    float *vertices;
    float *indices;

    Texture *textures;

    unsigned int index_count;
    
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
} Block;

Block *create_block();
void draw_block(Block *block);
void destroy_block(Block *block);

#endif // BLOCK_H
