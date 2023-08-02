#include "block.h"
#include "string.h"


void setup_block(Block *block, float block_size)
{
    float cube_vertices[] = {
        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int cube_indices[] = {
        0, 1, 2,  
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        1, 5, 6,
        6, 2, 1,
        0, 4, 7,
        7, 3, 0,
        3, 2, 6,
        6, 7, 3,
        0, 1, 5,
        5, 4, 0
    };

    // Create and bind the VAO
    block->vao = vao_create();
    vao_bind(block->vao);

    // Create and bind the VBO for vertices
    block->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    vbo_bind(block->vbo);
    vbo_buffer(block->vbo, cube_vertices, 0, sizeof(cube_vertices));

    // Create and bind the EBO for indices
    block->ebo = ebo_create();
    ebo_bind(block->ebo);
    ebo_buffer(block->ebo, cube_indices, sizeof(cube_indices), GL_STATIC_DRAW);

    vao_attr(block->vao, block->vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    // Store the number of vertices (number of indices for indexed rendering)
    block->num_vertices = sizeof(cube_indices) / sizeof(cube_indices[0]);
    
}

void draw_block(Block *block)
{
    vao_bind(block->vao);
    ebo_bind(block->ebo);
    glDrawElements(GL_TRIANGLES, block->num_vertices, GL_UNSIGNED_INT, 0);
}



bool is_chunk_visible(Camera *camera, int chunk_x, int chunk_z, int render_distance)
{
    return true;
}

void generate_visible_blocks(Camera *camera, Block *block)
{
    
}

    






