#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh process_mesh(const struct aiMesh *mesh, const struct aiScene *scene)
{
    Vertex *vertices = NULL;
    unsigned int *indices = NULL;

    unsigned int vertex_count = mesh->mNumVertices;
    vertices = (Vertex *)malloc(vertex_count * sizeof(Vertex));

    for (unsigned int i = 0; i < vertex_count; i++) {
        vec3 position = {0.0f, 0.0f, 0.0f};
        vec3 normal = {0.0f, 0.0f, 0.0f};
        vec2 texCoords = {0.0f, 0.0f};

        position[0] = mesh->mVertices[i].x;
        position[1] = mesh->mVertices[i].y;
        position[2] = mesh->mVertices[i].z;

        normal[0] = mesh->mNormals[i].x;
        normal[1] = mesh->mNormals[i].y;
        normal[2] = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            texCoords[0] = mesh->mTextureCoords[0][i].x;
            texCoords[1] = mesh->mTextureCoords[0][i].y;
        } else {
            texCoords[0] = 0.0f;
            texCoords[1] = 0.0f;
        }

        glm_vec3_copy(position, vertices[i].Position);
        glm_vec3_copy(normal, vertices[i].Normal);
        
    }

    unsigned int index_count = mesh->mNumFaces * 3;
    indices = (unsigned int *)malloc(index_count * sizeof(unsigned int));

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const struct aiFace *face = &mesh->mFaces[i];
        for (unsigned int j = 0; j < face->mNumIndices; j++) {
            indices[i * 3 + j] = face->mIndices[j];
        }
    }

    Mesh processed_mesh = create_mesh(vertices, vertex_count, indices, index_count);

    // Clean up allocated memory
    free(vertices);
    free(indices);

    return processed_mesh;
}

static void process_node(const struct aiScene *scene, const struct aiNode *node, Model *model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh processed_mesh = process_mesh(mesh, scene);
        model->meshes[model->mesh_count++] = processed_mesh;
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(scene, node->mChildren[i], model);
    }
}

Model *load_model(const char *filepath)
{
    const struct aiScene *scene = aiImportFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        fprintf(stderr, "Error loading model: %s\n", aiGetErrorString());
        return NULL;
    }

    Model *model = (Model *)malloc(sizeof(Model));
    model->mesh_count = 0;
    model->meshes = (Mesh *)malloc(scene->mNumMeshes * sizeof(Mesh));

    process_node(scene, scene->mRootNode, model);

    aiReleaseImport(scene);

    return model;
}    

void draw_model(Model *model)
{
    for (size_t i = 0; i < model->mesh_count; i++) {
        draw_mesh(&model->meshes[i]);
    }
}


