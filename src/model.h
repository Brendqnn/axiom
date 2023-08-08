#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"


typedef struct Model {
    Mesh *mesh;
} Model;

Model *load_model(const char* filepath);
void draw_model(Model *model);
void free_model(Model *model);

#endif // MODEL_H
