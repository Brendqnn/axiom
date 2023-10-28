#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"

typedef struct {
    vec3 position;
    vec3 normal;
    vec2 tex_coords;
} Vertex;

typedef struct {
    Vertex* vertices;
    unsigned int num_vertices;
    
    unsigned int *indices;
    unsigned int num_indices;

    Texture *textures;
    unsigned int num_textures;

    unsigned int VAO, VBO, EBO;
} Mesh;

Mesh* create_mesh(Vertex* vertices, unsigned int num_vertices,
                  unsigned int* indices, unsigned int num_indices, Texture *textures, unsigned int num_textures);
void draw_mesh(Mesh *mesh, Shader shader);
void destroy_mesh(Mesh *mesh);

#endif // MESH_H
