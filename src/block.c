#include "block.h"
#include "string.h"


void setup_block(Block *block) {
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2, // Front face - First triangle
        2, 3, 0, // Front face - Second triangle

        1, 5, 6, // Right face - First triangle
        6, 2, 1, // Right face - Second triangle

        7, 6, 5, // Back face - First triangle
        5, 4, 7, // Back face - Second triangle

        4, 0, 3, // Left face - First triangle
        3, 7, 4, // Left face - Second triangle

        4, 5, 1, // Bottom face - First triangle
        1, 0, 4, // Bottom face - Second triangle

        3, 2, 6, // Top face - First triangle
        6, 7, 3  // Top face - Second triangle
    };
    
    block->vao = vao_create();
    block->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    block->ebo = ebo_create();

    vao_bind(block->vao);
    vbo_buffer(block->vbo, vertices, 0, sizeof(vertices));
    ebo_buffer(block->ebo, indices, sizeof(indices), GL_STATIC_DRAW);

    vao_attr(block->vao, block->vbo, 0, 3, GL_FLOAT, 0, 0);
        
    // unbind here later
    
    block->block_size = 10.0f; 
}

void draw_block(Block *block) {
    vao_bind(block->vao);
    ebo_bind(block->ebo);

    glDrawElements(GL_TRIANGLES, sizeof(block->indices) / sizeof(block->indices[0]), GL_UNSIGNED_INT, 0);
}


