#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "gfx/stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static Texture* load_material_textures(const struct aiMaterial *mat);
static Texture texture_from_file(const char* filename, const char* typeName);
static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model);
static Mesh* process_mesh(struct aiMesh *ai_mesh, const struct aiScene *scene);


static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh *processed_mesh = process_mesh(mesh, scene);
        if (processed_mesh) {
            model->meshes = realloc(model->meshes, sizeof(Mesh*) * (model->num_meshes + 1));
            if (model->meshes) {
                model->meshes[model->num_meshes++] = processed_mesh;
            }
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, model); // Recursively process children nodes
    }
}

static Mesh* process_mesh(struct aiMesh *ai_mesh, const struct aiScene *scene)
{
    if (!ai_mesh->mVertices || !ai_mesh->mNumVertices || !ai_mesh->mFaces || !ai_mesh->mNumFaces) {
        return NULL;
    }

    Vertex *vertices = malloc(ai_mesh->mNumVertices * sizeof(Vertex));
    if (!vertices) {
        return NULL;
    }

    unsigned int *indices = malloc(ai_mesh->mNumFaces * 3 * sizeof(unsigned int));
    if (!indices) {
        free(vertices);
        return NULL;
    }

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        vertices[i].position[0] = ai_mesh->mVertices[i].x;
        vertices[i].position[1] = ai_mesh->mVertices[i].y;
        vertices[i].position[2] = ai_mesh->mVertices[i].z;

        if (ai_mesh->mNormals) {
            vertices[i].normal[0] = ai_mesh->mNormals[i].x;
            vertices[i].normal[1] = ai_mesh->mNormals[i].y;
            vertices[i].normal[2] = ai_mesh->mNormals[i].z;
        }

        if (ai_mesh->mTextureCoords[0]) {
            vertices[i].tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertices[i].tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertices[i].tex_coords[0] = 0.0f;
            vertices[i].tex_coords[1] = 0.0f;
        }
    }

    for (unsigned int i = 0, j = 0; i < ai_mesh->mNumFaces; i++) {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; k++) {
            indices[j++] = face.mIndices[k];
        }
    }

    
    Mesh *mesh = create_mesh(vertices, ai_mesh->mNumVertices, indices, ai_mesh->mNumFaces * 3, NULL);
    
    return mesh;
}

Model* load_model(const char* model_path)
{
    Model* model = malloc(sizeof(Model));
    if (!model) {
        return NULL;
    }
    model->num_meshes = 0;
    model->meshes = NULL;

    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        free(model);
        return NULL;
    }

    process_node(scene->mRootNode, scene, model);
    
    return model;
}

void draw_model(Model *model)
{
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        draw_mesh(model->meshes[i]);
    }
}

void destroy_model(Model *model)
{
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        destroy_mesh(model->meshes[i]);
    }

    free(model->meshes);
    free(model);
}
