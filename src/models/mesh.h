#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gfx/shader.h"
#include "../gfx/texture.h"

#define MAX_TEXTURES 25

typedef struct {
    float position[3];
    float normals[3];
    float tex_coords[2];
} Vertex;

typedef struct {
    unsigned int *indices;
    Vertex *vertices;
    Texture *textures;
    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_textures;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} Mesh;


Mesh create_mesh(Vertex vertices[], unsigned int indices[], Texture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);
void draw_mesh(Mesh mesh, Shader *shader);

#endif // MESH_H
