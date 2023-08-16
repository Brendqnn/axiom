#include "block.h"
#include <stdlib.h>
#include <string.h>

Block *create_block(float block_size)
{
    Block *block = malloc(sizeof(Block));
    if (block == NULL) {
        return NULL;
    }

    block->block_size = block_size;

    float vertex_coords[] = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
    };

    int index_coords[] = {
        0, 1, 2,
        2, 3, 0,
  
        1, 5, 6,
        6, 2, 1,
      
        7, 6, 5,
        5, 4, 7,
     
        4, 0, 3,
        3, 7, 4,
        
        3, 2, 6,
        6, 7, 3,
        
        4, 5, 1,
        1, 0, 4
    };

    memcpy(block->vertices, vertex_coords, sizeof(block->vertices));
    memcpy(block->indices, index_coords, sizeof(block->indices));

    block->vao = vao_create();
    vao_bind(block->vao);

    block->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    vbo_bind(block->vbo);
    vbo_buffer(block->vbo, block->vertices, 0, sizeof(block->vertices));
    //vbo_unbind();

    block->ebo = ebo_create();
    ebo_bind(block->ebo);
    ebo_buffer(block->ebo, block->indices, sizeof(block->indices), GL_STATIC_DRAW);
    ebo_unbind();

    vao_attr(block->vao, block->vbo, 0, 3, GL_FLOAT, 0, 0);
    
    return block;
}

void destroy_block(Block *block)
{
    
}

void draw_block(Block *block)
{
    vao_bind(block->vao); // Bind the VAO
    ebo_bind(block->ebo);
    // Issue drawing commands
    glDrawElements(GL_TRIANGLES, sizeof(block->indices) / sizeof(block->indices[0]), GL_UNSIGNED_INT, 0);
      
}
