#ifndef AXMESH_H
#define AXMESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gfx/axshader.h"
#include "../gfx/axtexture.h"

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
    AXTexture *textures;
    Vertex *vertices;
} AXMesh;

AXMesh ax_create_mesh(Vertex vertices[], unsigned int indices[], AXTexture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);
void ax_draw_mesh(AXMesh mesh, AXShader *shader);

#endif // AXMESH_H
