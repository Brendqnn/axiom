#ifndef MODEL_H
#define MODEL_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Mesh mesh;
    const char* dir;
} Model;

void draw(Shader &shader);
void load_model(const char* path);
void process_node(aiNode *node, const aiScene *scene);
void process_mesh(aiMesh *mesh, const aiScene *scene);

#endif // MODEL_H
