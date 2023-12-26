#include "model.h"

Model load_model(const char* model_path)
{
    Model model = { 0 };
    
    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        return model;
    }

    //model.meshes = malloc(scene->mNumMeshes * sizeof(Mesh));
    
    //process_node(scene->mRootNode, scene, &model);

    aiReleaseImport(scene);
    
    return model;
}

void process_node(const struct aiNode* node, const struct aiScene* scene, Model *model)
{    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const struct aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        Mesh processed_mesh = process_mesh(aiMesh, scene);
        model->meshes[model->num_meshes++] = processed_mesh;
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene, model);
    }
}

Mesh process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene)
{
    Mesh mesh;

    Vertex vertices[4096];    
    unsigned int indices[6144];   
    Texture textures[50];

    mesh.num_vertices = ai_mesh->mNumVertices;

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
        vertices->position[0] = ai_mesh->mVertices[i].x;
        vertices->position[1] = ai_mesh->mVertices[i].y;
        vertices->position[2] = ai_mesh->mVertices[i].z;

        vertices->normals[0] = ai_mesh->mNormals[i].x;
        vertices->normals[1] = ai_mesh->mNormals[i].y;
        vertices->normals[2] = ai_mesh->mNormals[i].z;

        if (ai_mesh->mTextureCoords[0]) {
            vertices->tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertices->tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertices->tex_coords[0] = 0.0f;
            vertices->tex_coords[1] = 0.0f;
        }
    }
    
    mesh = create_mesh(vertices, indices, textures, 0, mesh.num_vertices, 0);
    
    return mesh;
}

void draw_model(Model *model, Shader shader)
{
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        draw_mesh(model->meshes[i], shader);
    }
}

