#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "vbo.h"
#include "ibo.h"
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>


typedef struct {
    vec3 position;
    vec3 normal;
    vec2 tex_coords; 
} Vertex;

typedef struct {
    Vertex* vertices;
    unsigned int* indices;
    unsigned int num_vertices;
    unsigned int num_indices;
    struct VAO vao;
    struct VBO vbo;
    struct IBO ibo; 
} Mesh;

Mesh* setup_mesh(Vertex* vertices, unsigned int* indices, unsigned int num_vertices, unsigned int num_indices);
void draw(Mesh* mesh);
void mesh_destroy(Mesh* mesh);

#endif /* MESH_H */


