#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <string.h>

#include "vao.h"
#include "vbo.h"
#include "ebo.h"


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

    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;
    
} Mesh;

Mesh* create_mesh(Vertex* vertices, unsigned int num_vertices, GLuint* indices, unsigned int num_indices);
void draw_mesh(Mesh *mesh);
void destroy_mesh(Mesh *mesh);

#endif // MESH_H
    
