#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static Mesh* process_mesh(struct aiMesh *ai_mesh);

static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh *mesh = process_mesh(ai_mesh);
        if (mesh) {
            model->meshes[model->num_meshes++] = mesh;
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, model);
    }
}

static Mesh* process_mesh(struct aiMesh *ai_mesh) {
    // Check if the mesh has vertices and indices
    if (!ai_mesh->mVertices || !ai_mesh->mNumVertices || !ai_mesh->mFaces || !ai_mesh->mNumFaces) {
        return NULL;
    }

    // Create an array of Vertex structures
    Vertex *vertices = malloc(ai_mesh->mNumVertices * sizeof(Vertex));
    if (!vertices) {
        return NULL;
    }

    // Populate the vertices with data from ai_mesh
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        // You need to adapt this part to match your data format
        vertices[i].position[0] = ai_mesh->mVertices[i].x;
        vertices[i].position[1] = ai_mesh->mVertices[i].y;
        vertices[i].position[2] = ai_mesh->mVertices[i].z;

    }

    // Create an array for indices
    GLuint *indices = malloc(ai_mesh->mNumFaces * 3 * sizeof(GLuint));
    if (!indices) {
        free(vertices);
        return NULL;
    }

    // Populate indices with data from ai_mesh
    for (unsigned int i = 0, j = 0; i < ai_mesh->mNumFaces; i++) {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; k++) {
            indices[j++] = face.mIndices[k];
        }
    }

    // Create and initialize a Mesh using the created data
    Mesh *mesh = create_mesh(vertices, ai_mesh->mNumVertices, indices, ai_mesh->mNumFaces * 3);

    // Clean up allocated memory
    free(vertices);
    free(indices);

    return mesh;
}

Model* load_model(const char* model_path) {
    Model* model = malloc(sizeof(Model));
    if (!model) {
        return NULL;
    }
    model->num_meshes = 0;

    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        free(model);
        return NULL;
    }

    process_node(scene->mRootNode, scene, model);

    aiReleaseImport(scene);

    return model;
}

void draw_model(Model *model) {
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        draw_mesh(model->meshes[i]);
    }
}

void destroy_model(Model *model) {
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        destroy_mesh(model->meshes[i]);
    }

    free(model->meshes);
    free(model);
}

