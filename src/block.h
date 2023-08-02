#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>
#include <stdlib.h>

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "camera.h"

#define CHUNK_SIZE 16 // Size of each chunk containing blocks

typedef struct Block {
    int render_distance;
    float vertices[1000];
    int num_vertices;
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
    int world_x;
    int world_z;
} Block;

void setup_block(Block *block, float block_size);
void generate_visible_blocks(Camera *camera, Block *block);
void draw_block(Block *block);

#endif // BLOCK_H
