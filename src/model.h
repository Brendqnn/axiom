#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

typedef struct {
    Mesh* meshes; 
    unsigned int num_meshes; 
   
} Model;

Model* load_model(const char* model_path);
void draw_model(Model* model);
void model_destroy(Model* model);

#endif /* MODEL_H */


