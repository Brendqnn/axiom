#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static void process_node(const struct aiScene *scene, const struct aiNode *node, Model *model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        Vertex *vertices = malloc(sizeof(Vertex) * mesh->mNumVertices);
        unsigned int *indices = malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            // Vertex positions
            vertices[j].Position[0] = mesh->mVertices[j].x;
            vertices[j].Position[1] = mesh->mVertices[j].y;
            vertices[j].Position[2] = mesh->mVertices[j].z;

            // Vertex normals
            if (mesh->mNormals) {
                vertices[j].Normal[0] = mesh->mNormals[j].x;
                vertices[j].Normal[1] = mesh->mNormals[j].y;
                vertices[j].Normal[2] = mesh->mNormals[j].z;
            }

            // Print vertex information for debugging
            printf("Vertex %u:\n", j);
            printf("  Position: %.3f %.3f %.3f\n", vertices[j].Position[0], vertices[j].Position[1], vertices[j].Position[2]);
            if (mesh->mNormals) {
                printf("  Normal: %.3f %.3f %.3f\n", vertices[j].Normal[0], vertices[j].Normal[1], vertices[j].Normal[2]);
            }
        }

        unsigned int indexOffset = 0;
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            struct aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                indices[indexOffset++] = face.mIndices[k];
            }
        }

        printf("Creating mesh %u\n", model->mesh_count);
        model->meshes[model->mesh_count++] = create_mesh(vertices, mesh->mNumVertices, indices, mesh->mNumFaces * 3);

        free(indices);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(scene, node->mChildren[i], model);
    }
}

Model *load_model(const char *filepath)
{
    Model *model = malloc(sizeof(Model));
    model->mesh_count = 0;

    const struct aiScene *scene = aiImportFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Error loading model: %s\n", aiGetErrorString());
        free(model);
        return NULL;
    }

    model->meshes = malloc(sizeof(Mesh) * scene->mNumMeshes);

    process_node(scene, scene->mRootNode, model);

    aiReleaseImport(scene);

    return model;
}

void draw_model(Model *model)
{
    for (size_t i = 0; i < model->mesh_count; i++) {
        draw_mesh(model->meshes[i]);
    }
}




