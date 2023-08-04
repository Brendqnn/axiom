#ifndef CHUNK_H
#define CHUNK_H

#include <stdbool.h>
#include <stdlib.h>

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "camera.h"

#define CHUNK_SIZE 12 // Size of each chunk containing blocks
#define BLOCK_SIZE 1.0f

typedef struct Chunk {
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
    
    int world_x;
    int world_z;

    int num_vertices;
    
    bool generated;
} Chunk;

void setup_chunk(Chunk *chunk);
void generate_visible_blocks(Camera *camera, Chunk chunks[][CHUNK_SIZE]);
void draw_chunk(Chunk *chunk);

#endif // CHUNK_H
