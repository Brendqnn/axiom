#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model* load_model(const char* model_path) {
    Model* model = (Model*)malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Failed to allocate memory for Model.\n");
        return NULL;
    }
    
    // Create an Assimp importer.
    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "Assimp import error: %s\n", aiGetErrorString());
        free(model);
        return NULL;
    }

    // Process each mesh in the model and create a Mesh component for each.
    model->num_meshes = scene->mNumMeshes;

    // Allocate memory for storing Mesh components in the Model.
    model->meshes = (Mesh*)malloc(model->num_meshes * sizeof(Mesh));
    if (!model->meshes) {
        fprintf(stderr, "Failed to allocate memory for Mesh array in Model.\n");
        aiReleaseImport(scene);
        free(model);
        return NULL;
    }

    for (unsigned int i = 0; i < model->num_meshes; i++) {
        model->meshes[i] = setup_mesh(NULL, NULL, 0, 0);

        Vertex* vertices = (Vertex*)malloc(mesh->mNumVertices * sizeof(Vertex));
        if (!vertices) {
            printf("failed to allocate vertices\n");
            aiReleaseImport(scene);
            model_destroy(model); // Ensure proper cleanup in case of allocation failure
            return NULL;
        }
        unsigned int* indices = (unsigned int*)malloc(mesh->mNumFaces * 3 * sizeof(unsigned int));
        if (!indices) {
            printf("failed to allocate indices\n");
            free(vertices); // Proper cleanup before returning NULL
            aiReleaseImport(scene);
            model_destroy(model);
            return NULL;
        }

        // Copy vertex data from Assimp mesh to the allocated vertices array
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            vertices[j].position[0] = mesh->mVertices[j].x;
            vertices[j].position[1] = mesh->mVertices[j].y;
            vertices[j].position[2] = mesh->mVertices[j].z;

            if (mesh->mNormals) {
                vertices[j].normal[0] = mesh->mNormals[j].x;
                vertices[j].normal[1] = mesh->mNormals[j].y;
                vertices[j].normal[2] = mesh->mNormals[j].z;
            }
            if (mesh->mTextureCoords[0]) {
                vertices[j].tex_coords[0] = mesh->mTextureCoords[0][j].x;
                vertices[j].tex_coords[1] = mesh->mTextureCoords[0][j].y;
            }
        }

        // Copy index data from Assimp mesh to the allocated indices array
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            const struct aiFace* face = &mesh->mFaces[j];
            if (face->mNumIndices != 3) {
                fprintf(stderr, "Error: Only triangular faces are supported.\n");
                free(vertices);
                free(indices);
                aiReleaseImport(scene);
                free(model);
                return NULL;
            }

            // Copy face indices to the indices array.
            indices[j * 3] = face->mIndices[0];
            indices[j * 3 + 1] = face->mIndices[1];
            indices[j * 3 + 2] = face->mIndices[2];
        }

        // Create and set up the mesh using the setup_mesh function.
        model->meshes[i] = *setup_mesh(vertices, indices, mesh->mNumVertices, mesh->mNumFaces * 3);

        // Free the vertices and indices arrays as they are now stored in the mesh.
        free(vertices);
        free(indices);
    }

    // Release the Assimp scene.
    aiReleaseImport(scene);

    return model;
}

void draw_model(Model* model) {
    if (!model)
        return;

    for (unsigned int i = 0; i < model->num_meshes; i++) {
        draw(&model->meshes[i]);
    }
}

void model_destroy(Model* model) {
    if (!model)
        return;

    // Free each mesh in the model.
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        mesh_destroy(&model->meshes[i]);
    }

    // Free the meshes array.
    free(model->meshes);
    free(model);
}




