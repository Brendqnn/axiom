#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "camera.h"

typedef struct Block {
    float block_size;
    
    float vertices[30000]; // Each cube has 36 vertices (6 faces * 6 vertices per face)
    int num_vertices;
    
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
} Block;

void setup_block(Block *block, float block_size, int num_squares);
void generate_visible_blocks(Camera *camera, int render_distance, Block* blocks, int num_squares);
void draw_block(Block *block);

#endif // BLOCK_H

