#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include "mesh.h"
#include "shader.h"


struct Model
{
    struct Mesh* meshes;
    int numMeshes;
};

struct Model* loadModel(const char* filePath);
void drawModel(struct Model* model, Shader shader);
void destroyModel(struct Model* model);

#endif // MODEL_H




