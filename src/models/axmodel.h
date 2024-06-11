#ifndef AXMODEL_H
#define AXMODEL_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include "../util/axutil.h"

#include "axmesh.h"

#define MAX_MESHES 100

typedef struct {
    AXMesh meshes[MAX_MESHES];
    unsigned int num_meshes;

    mat4 matrix; 
} AXModel;

AXModel ax_load_model(const char* path);
void ax_process_node(const struct aiNode* node, const struct aiScene* scene, AXModel *model);
void ax_process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, AXModel *model);
void ax_draw_model(AXModel model, AXShader *shader);
void ax_free_model(AXModel *model);

#endif // AXMODEL_H
