#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include "mesh.h"


struct Model
{
    struct Mesh* meshes;
    char* directory;
    struct Texture* textures_loaded;
    int numMeshes;
};

struct Model* createModel(const char* path);

void DrawModel(struct Model* model, GLuint shader);

void destroyModel(struct Model* model);

#endif // MODEL_H




