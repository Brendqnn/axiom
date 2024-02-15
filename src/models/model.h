#ifndef MODEL_H
#define MODEL_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include "../util/util.h"

#include "mesh.h"

#define MAX_MESHES 100

typedef struct {
    Mesh meshes[MAX_MESHES];
    unsigned int num_meshes;

    mat4 matrix; 
} Model;

/**
 * Load a 3D model from the specified file path.
 *
 * @param path The file path of the model.
 *
 * @return A Model structure representing the loaded model.
 */
Model load_model(const char* path);

/**
 * Process the nodes of the loaded model.
 *
 * @param node  The root node of the model.
 * @param scene The Assimp scene structure.
 * @param model The Model structure to populate.
 */
void process_node(const struct aiNode* node, const struct aiScene* scene, Model *model);

/**
 * Process a mesh in the model.
 *
 * @param ai_mesh The Assimp mesh structure.
 * @param scene   The Assimp scene structure.
 * @param model   The Model structure to populate.
 */
void process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, Model *model);

/**
 * Draw the loaded model using the specified shader.
 *
 * @param model  The Model to be drawn.
 * @param shader The shader program to use for rendering.
 */
void draw_model(Model model, Shader *shader);


void free_model(Model *model);

#endif // MODEL_H
