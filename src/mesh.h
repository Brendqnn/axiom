#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "texture.h"

#define MAX_VERTICES 60000
#define MAX_INDICES 212500
#define MAX_TEXTURES 50

typedef struct {
    float position[3];
    float normals[3];
    float tex_coords[2];
} Vertex;

typedef struct {
    Vertex vertices[MAX_VERTICES];    
    unsigned int indices[MAX_INDICES];   
    Texture textures[MAX_TEXTURES];

    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_textures;
    
    unsigned int VAO, VBO, EBO;
} Mesh;

Mesh create_mesh(Vertex vertices[], unsigned int indices[], Texture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);
void draw_mesh(Mesh mesh, Shader shader);

#endif // MESH_H
