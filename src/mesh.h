#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "vbo.h"
#include "ibo.h"


typedef struct {
    float position[3];
    float normal[3];
} Vertex;

typedef struct {
    Vertex* vertices;
    unsigned int* indices;
    
    unsigned int num_vertices;
    unsigned int num_indices;
    
    // render data
    struct VAO vao = vao_create();
    struct VBO vbo = vbo_create(GL_ARRAY_BUFFER, false);
    struct IBO ibo = ibo_create();
        
} Mesh;

Mesh* create_mesh(Vertex* vertices, unsigned int* indices, unsigned int num_vertices, unsigned int num_indices);                
void draw();
void mesh_destroy();

#endif /* MESH_H */
