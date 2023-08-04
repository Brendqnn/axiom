#include "chunk.h"
#include "string.h"

void setup_chunk(Chunk *chunk)
{
    int vertex_offset = 0;
    
    for (size_t z = 0; z < CHUNK_SIZE; z++) {
        for (size_t x = 0; x < CHUNK_SIZE; x++) {
            float cube_vertices[] = {
                // Calculate the position of each cube based on its world coordinates
                (float)(x) * BLOCK_SIZE, 0.0f, (float)(z) * BLOCK_SIZE,             // Vertex 1 (x, y, z)
                (float)(x + 1) * BLOCK_SIZE, 0.0f, (float)(z) * BLOCK_SIZE,         // Vertex 2 (x, y, z)
                (float)(x + 1) * BLOCK_SIZE, 0.0f, (float)(z + 1) * BLOCK_SIZE,     // Vertex 3 (x, y, z)
                (float)(x) * BLOCK_SIZE, 0.0f, (float)(z + 1) * BLOCK_SIZE,         // Vertex 4 (x, y, z)

                // Vertices for the top face of the cube (y = 1.0)
                (float)(x) * BLOCK_SIZE, 1.0f, (float)(z) * BLOCK_SIZE,             // Vertex 5 (x, y, z)
                (float)(x + 1) * BLOCK_SIZE, 1.0f, (float)(z) * BLOCK_SIZE,         // Vertex 6 (x, y, z)
                (float)(x + 1) * BLOCK_SIZE, 1.0f, (float)(z + 1) * BLOCK_SIZE,     // Vertex 7 (x, y, z)
                (float)(x) * BLOCK_SIZE, 1.0f, (float)(z + 1) * BLOCK_SIZE,         // Vertex 8 (x, y, z)
            };

            unsigned int cube_indices[] = {
                // Indices for the bottom face of the cube (drawn counter-clockwise)
                0 + vertex_offset, 1 + vertex_offset, 2 + vertex_offset,
                2 + vertex_offset, 3 + vertex_offset, 0 + vertex_offset,

                // Indices for the top face of the cube (drawn counter-clockwise)
                4 + vertex_offset, 5 + vertex_offset, 6 + vertex_offset,
                6 + vertex_offset, 7 + vertex_offset, 4 + vertex_offset,
            };

            chunk->vao = vao_create();
            vao_bind(chunk->vao);

            chunk->vbo = vbo_create(GL_ARRAY_BUFFER, false);
            vbo_bind(chunk->vbo);
            vbo_buffer(chunk->vbo, cube_vertices, 0, sizeof(cube_vertices));

            chunk->ebo = ebo_create();
            ebo_bind(chunk->ebo);
            ebo_buffer(chunk->ebo, cube_indices, sizeof(cube_indices), GL_STATIC_DRAW);

            vao_attr(chunk->vao, chunk->vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

            chunk->num_vertices = sizeof(cube_indices) / sizeof(cube_indices[0]);

            vertex_offset += 8;
        }
    }

    chunk->generated = true;
}

void draw_chunk(Chunk *chunk)
{
    vao_bind(chunk->vao);
    ebo_bind(chunk->ebo);
    glDrawElements(GL_TRIANGLES, chunk->num_vertices, GL_UNSIGNED_INT, 0);
}


bool is_chunk_visible(Camera *camera, int chunk_x, int chunk_z)
{
    int camera_chunk_x = (int)(camera->position[0]) / CHUNK_SIZE;
    int camera_chunk_z = (int)(camera->position[2]) / CHUNK_SIZE;

    int dx = abs(chunk_x - camera_chunk_x);
    int dz = abs(chunk_z - camera_chunk_z);

    return (dx <= camera->render_distance) && (dz <= camera->render_distance);
}

void generate_visible_blocks(Camera *camera, Chunk chunks[][CHUNK_SIZE])
{
    int camera_chunk_x = (int)(camera->position[0]) / CHUNK_SIZE;
    int camera_chunk_z = (int)(camera->position[2]) / CHUNK_SIZE;

    // Calculate the range of chunk coordinates that are potentially visible
    int min_chunk_x = camera_chunk_x - camera->render_distance;
    int max_chunk_x = camera_chunk_x + camera->render_distance;
    int min_chunk_z = camera_chunk_z - camera->render_distance;
    int max_chunk_z = camera_chunk_z + camera->render_distance;

    // Iterate through all chunks and mark them as generated if they are within the render distance
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int chunk_x = camera_chunk_x + x - (CHUNK_SIZE / 2);
            int chunk_z = camera_chunk_z + z - (CHUNK_SIZE / 2);

            // Check if the chunk is within the render distance and is not already generated
            if (chunk_x >= min_chunk_x && chunk_x <= max_chunk_x && chunk_z >= min_chunk_z && chunk_z <= max_chunk_z) {
                if (!chunks[z][x].generated) {
                    setup_chunk(&chunks[z][x]);
                    draw_chunk(&chunks[z][x]);
                }
                
            } else {
                // Unload the chunk if it's not visible and is currently generated
                if (chunks[z][x].generated) {
                    
                    chunks[z][x].generated = false;
                }
            }
        }
    }
}

void unload_chunks_outside_distance(Camera *camera, Chunk chunks[][CHUNK_SIZE])
{
    int camera_chunk_x = (int)(camera->position[0]) / CHUNK_SIZE;
    int camera_chunk_z = (int)(camera->position[2]) / CHUNK_SIZE;

    for (int dz = -camera->render_distance - 1; dz <= camera->render_distance + 1; dz++) {
        for (int dx = -camera->render_distance - 1; dx <= camera->render_distance + 1; dx++) {
            int chunk_x = camera_chunk_x + dx;
            int chunk_z = camera_chunk_z + dz;

            if (is_chunk_visible(camera, chunk_x, chunk_z))
                continue; // Don't unload visible chunks

            // Unload the chunk if it's not visible
            if (chunk_x >= 0 && chunk_x < CHUNK_SIZE && chunk_z >= 0 && chunk_z < CHUNK_SIZE) {
                if (chunks[chunk_z][chunk_x].generated) {
                    
                }
            }
        }
    }
}


    






