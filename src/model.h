#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/util.h"
#include "mesh.h"

typedef struct {
    Mesh **meshes;
    unsigned int num_meshes;
} Model;

Model *load_model(const char *model_path);
void draw_model(Model *model, Shader shader);
void destroy_model(Model *model, Shader shader);

#endif // MODEL_H

