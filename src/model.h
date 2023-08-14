#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

typedef struct Model {
    Mesh *meshes; 
    unsigned int mesh_count;
} Model;

Model *load_model(const char* filepath);
void draw_model(Model *model);

#endif // MODEL_H

