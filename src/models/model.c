#include "model.h"


Texture* load_material_textures(struct aiMaterial* material, enum aiTextureType texture_type, const char* texture_type_name)
{
    unsigned int texture_count = aiGetMaterialTextureCount(material, texture_type);
    if (texture_count == 0) {
        return NULL;
    }

    Texture* textures = (Texture*)malloc(texture_count * sizeof(Texture));
    for (unsigned int i = 0; i < texture_count; ++i) {
        struct aiString texture_path;
        if (aiGetMaterialTexture(material, texture_type, i, &texture_path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            textures[i] = load_model_texture(texture_path.data, texture_type_name);
            textures[i].count = texture_count;
        }
    }
    return textures;
}

Model load_model(const char* model_path)
{
    Model model;
    
    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        return model;
    }

    process_node(scene->mRootNode, scene, &model);
    
    aiReleaseImport(scene);
    return model;
}

void process_node(const struct aiNode* node, const struct aiScene* scene, Model *model)
{    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const struct aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(aiMesh, scene, model);
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene, model);
    }
}

void process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, Model *model)
{
    unsigned int num_vertices = ai_mesh->mNumVertices;
    unsigned int num_indices = 0;
    unsigned int num_textures = 0;
    
    Vertex* vertices = (Vertex*)calloc(num_vertices, sizeof(Vertex));

    for (unsigned int i = 0; i < num_vertices; ++i) {
        vertices[i].position[0] = ai_mesh->mVertices[i].x;
        vertices[i].position[1] = ai_mesh->mVertices[i].y;
        vertices[i].position[2] = ai_mesh->mVertices[i].z;

        vertices[i].normals[0] = ai_mesh->mNormals[i].x;
        vertices[i].normals[1] = ai_mesh->mNormals[i].y;
        vertices[i].normals[2] = ai_mesh->mNormals[i].z;

        if (ai_mesh->mTextureCoords[0]) {
            vertices[i].tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertices[i].tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertices[i].tex_coords[0] = 0.0f;
            vertices[i].tex_coords[1] = 0.0f;
        }
    }

    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        num_indices += ai_mesh->mFaces[i].mNumIndices;
    }

    unsigned int* indices = (unsigned int*)malloc(num_indices * sizeof(unsigned int));

    unsigned int idx = 0;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; ++k) {
            indices[idx++] = face.mIndices[k];
        }
    }

    Texture *textures = NULL;

    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
        Texture* diffuse_textures = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse1");
        num_textures = diffuse_textures->count;
        textures = diffuse_textures;
    } 

    Mesh processed_mesh = create_mesh(vertices, indices, textures, num_vertices, num_indices, num_textures);
    model->meshes[model->num_meshes++] = processed_mesh;

    free(vertices);
    free(indices);
}

void draw_model(Model model, Shader *shader)
{
    glm_mat4_identity(model.matrix);

    for (unsigned int i = 0; i < model.num_meshes; i++) {
        draw_mesh(model.meshes[i], shader);
    }
}

void free_model(Model *model)
{
    free(model->meshes->textures);
    free(model->meshes);
}


