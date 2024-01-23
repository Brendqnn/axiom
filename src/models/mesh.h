#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gfx/shader.h"
#include "../gfx/texture.h"

typedef struct {
    float position[3];
    float normals[3];
    float tex_coords[2];
} Vertex;

typedef struct {
    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_textures;

    unsigned int VAO, VBO, EBO;

    unsigned int *indices;
    Texture *textures;
    Vertex *vertices;
} Mesh;

/**
 * Create a Mesh with specified vertices, indices, and textures.
 *
 * @param vertices      Array of vertices for the mesh.
 * @param indices       Array of indices defining the mesh geometry.
 * @param textures      Array of textures associated with the mesh.
 * @param num_vertices  Number of vertices in the vertices array.
 * @param num_indices   Number of indices in the indices array.
 * @param num_textures  Number of textures in the textures array.
 *
 * @return A Mesh structure representing the created mesh.
 */
Mesh create_mesh(Vertex vertices[], unsigned int indices[], Texture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);

/**
 * Draw the Mesh using the specified shader.
 *
 * @param mesh   The Mesh to be drawn.
 * @param shader The shader program to use for rendering.
 */
void draw_mesh(Mesh mesh, Shader *shader);

#endif // MESH_H
