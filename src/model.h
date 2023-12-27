#ifndef MODEL_H
#define MODEL_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdbool.h>

#include "mesh.h"

typedef struct {
    Mesh *meshes;
    unsigned int num_meshes;
} Model;

Model load_model(const char* path);
void process_node(const struct aiNode* node, const struct aiScene* scene, Model *model);
void process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, Model *model);
void draw_model(Model model, Shader shader);

#endif // MODEL_H
