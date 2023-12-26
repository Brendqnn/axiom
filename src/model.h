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

/**
 * Load a 3D model from a file using Assimp.
 *
 * @param path The file path of the model.
 * @return A Model structure representing the loaded model.
 */

Model load_model(const char* path);

/**
 * Process a node recursively and populate the meshes array.
 *
 * @param node The Assimp node to process.
 * @param scene The Assimp scene containing the node.
 * @return An array of Mesh structures.
 */

void process_node(const struct aiNode* node, const struct aiScene* scene, Model *model);

/**
 * Process a mesh and fill the mesh structure with data.
 *
 * @param mesh The Assimp mesh to process.
 * @param scene The Assimp scene containing the mesh.
 * @return A Mesh structure representing the processed mesh.
 */

Mesh process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene);

/**
 * Process a mesh and fill the mesh structure with data.
 *
 * @param model Model obj to render.
 * @param shader The shader used to draw the model.
 */

void draw_model(Model *model, Shader shader);

#endif // MODEL_H
