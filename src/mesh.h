#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <cglm/cglm.h>

#include "vao.h"
#include "vbo.h"
#include "ebo.h"

typedef struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} Vertex;

typedef struct Mesh {
    struct VAO vao;
    struct VBO vbo;
    struct EBO ebo;

    Vertex *vertices;
    unsigned int *indices;
    unsigned int vertex_count;
    unsigned int index_count;
} Mesh;

Mesh create_mesh(Vertex *vertices, unsigned int vertex_count, unsigned int *indices, unsigned int index_count);
void destroy_mesh(Mesh *mesh);
void draw_mesh(Mesh *mesh);

#endif // MESH_H


