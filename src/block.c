#include "block.h"
#include <string.h>
#include <math.h>

void setup_block(Block *block, float block_size, int num_squares) {
    float cube_size = block_size / num_squares;
    int num_vertices = 36 * num_squares * num_squares; // 36 vertices per cube

    // Fill the vertices array with positions for each cube
    int vertex_index = 0;

    for (int row = 0; row < num_squares; row++) {
        for (int col = 0; col < num_squares; col++) {
            // Calculate the position of the current cube
            float x = col * cube_size;
            float y = 0.0f; // Set the base y-coordinate for the cube (bottom face)
            float z = row * cube_size;

            // Define the vertices for the current cube
            float cube_vertices[] = {
                // Front face
                x, y, z,
                x + cube_size, y, z,
                x + cube_size, y + cube_size, z,
                x, y, z,
                x + cube_size, y + cube_size, z,
                x, y + cube_size, z,

                // Right face
                x + cube_size, y, z,
                x + cube_size, y, z + cube_size,
                x + cube_size, y + cube_size, z + cube_size,
                x + cube_size, y, z,
                x + cube_size, y + cube_size, z + cube_size,
                x + cube_size, y + cube_size, z,

                // Back face
                x + cube_size, y, z + cube_size,
                x, y, z + cube_size,
                x, y + cube_size, z + cube_size,
                x + cube_size, y, z + cube_size,
                x, y + cube_size, z + cube_size,
                x + cube_size, y + cube_size, z + cube_size,

                // Left face
                x, y, z + cube_size,
                x, y, z,
                x, y + cube_size, z,
                x, y, z + cube_size,
                x, y + cube_size, z,
                x, y + cube_size, z + cube_size,

                // Top face
                x, y + cube_size, z,
                x + cube_size, y + cube_size, z,
                x + cube_size, y + cube_size, z + cube_size,
                x, y + cube_size, z,
                x + cube_size, y + cube_size, z + cube_size,
                x, y + cube_size, z + cube_size,

                // Bottom face
                x, y, z + cube_size,
                x + cube_size, y, z + cube_size,
                x + cube_size, y, z,
                x, y, z + cube_size,
                x + cube_size, y, z,
                x, y, z
            };

            // Copy the vertices for the current cube to the array
            memcpy(block->vertices + vertex_index, cube_vertices, sizeof(cube_vertices));

            // Update the indices for the next cube
            vertex_index += sizeof(cube_vertices) / sizeof(float);
        }
    }

    // Create and bind the VAO, VBO
    block->vao = vao_create();
    block->vbo = vbo_create(GL_ARRAY_BUFFER, false);

    vao_bind(block->vao);
    vbo_buffer(block->vbo, block->vertices, 0, sizeof(float) * vertex_index);

    // Set up attribute pointers for position data
    vao_attr(block->vao, block->vbo, 0, 3, GL_FLOAT, 0, 0); // Position attribute

    // Store the number of vertices
    block->num_vertices = num_vertices;
    block->block_size = block_size;
}
 
void draw_block(Block *block)
{
    vao_bind(block->vao);

    glDrawArrays(GL_TRIANGLES, 0, block->num_vertices);
}




