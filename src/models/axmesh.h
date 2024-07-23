#ifndef AXMESH_H
#define AXMESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

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
    unsigned int instanceVBO;
    unsigned int instance_count;

    void *model_matrices;

    AXTexture *textures;
    Vertex *vertices;
    unsigned int *indices;

    bool instance;
} AXMesh;

AXMesh ax_create_mesh(Vertex vertices[], unsigned int indices[], AXTexture* textures,
                      unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);
AXMesh ax_create_mesh_instances(Vertex *vertices, unsigned int* indices, AXTexture* textures,
                      unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures);
void ax_setup_model_instance(void *model_matrices, unsigned int instance_count);
void ax_draw_mesh(AXMesh mesh, AXShader* shader);
void ax_draw_mesh2(AXMesh mesh, AXShader* shader);
void ax_draw_mesh_instances(AXMesh mesh, AXShader* shader, unsigned int instance_count);
void ax_free_mesh(AXMesh mesh);

#endif // AXMESH_H
