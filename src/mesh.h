#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include "vao.h"
#include "vbo.h"
#include "ibo.h"


typedef struct Vertex { 
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} Vertex;

typedef struct Mesh {
    Vertex* vertices;
    unsigned int *indices;
    unsigned int num_vertices;
    unsigned int num_indices;
} Mesh;

Mesh* create_mesh(Vertex* vertices);
void draw(Mesh* mesh);
void setup_mesh(Mesh* mesh);

#endif // MESH_H



