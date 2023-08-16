#include "block.h"


Block *create_block()
{
    Block *block = malloc(sizeof(Block));
    
    float vertices[] = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
    };

    unsigned int indices[] = {
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

    // Dynamically allocate memory for vertices and indices
    block->vertices = malloc(sizeof(vertices));
    memcpy(block->vertices, vertices, sizeof(vertices));

    block->indices = malloc(sizeof(indices));
    memcpy(block->indices, indices, sizeof(indices));

    block->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    block->vao = vao_create();
    block->ebo = ebo_create();

    vao_bind(block->vao);

    vbo_buffer(block->vbo, block->vertices, 0, sizeof(vertices));
    vao_attr(block->vao, block->vbo, 0, 3, GL_FLOAT, 0, 0);

    ebo_buffer(block->ebo, block->indices, sizeof(indices), GL_STATIC_DRAW);
    block->index_count = sizeof(indices) / sizeof(indices[0]);

    // Setup textures
    for (size_t i = 0; i < 6; i++) {
        block->textures[i] = texture_load("res/dirt.jpg", true);
    }
    
    return block;
}

void draw_block(Block *block)
{
    vao_bind(block->vao);
    ebo_bind(block->ebo);

    for (size_t i = 0; i < 6; i++) {
        texture_bind(&block->textures[i], 0);
        glDrawElements(GL_TRIANGLES, block->index_count, GL_UNSIGNED_INT, 0);
        texture_unbind(&block->textures[i]); 
    }
    
}

void destroy_block(Block *block)
{
    
}
