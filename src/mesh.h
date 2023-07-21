#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "vbo.h"
#include "ibo.h"


typedef struct {
    float position[3];
    float normal[3];
    float texCoords[2];
} Vertex;

typedef struct {
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;

    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_textures;

    // render data
    struct VAO vao = vao_create();
    struct VBO vbo = vbo_create(GL_ARRAY_BUFFER, false);
    struct IBO ibo = ibo_create();
        
} Mesh;

Mesh* create_mesh(Vertex* vertices, unsigned int* indices, Texture* textures,
                 unsigned int numVertices, unsigned int numIndices, unsigned int numTextures);
void draw(Shader &shader);
void setup_mesh();

#endif /* MESH_H */
