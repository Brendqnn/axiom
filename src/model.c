#include "model.h"

Model load_model(const char* model_path)
{
    Model model = { 0 };
    
    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate
                                               | aiProcess_FlipUVs
                                               | aiProcess_OptimizeMeshes
                                               | aiProcess_SplitLargeMeshes
                                               | aiProcess_OptimizeGraph
                                               | aiProcess_JoinIdenticalVertices
                                               | aiProcess_RemoveRedundantMaterials
                                               );
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        return model;
    }

    model.meshes = (Mesh*)malloc(scene->mNumMeshes * sizeof(Mesh)); // alloc henuff memory for meshes
    
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
    Mesh processed_mesh;

    Vertex vertices[MAX_VERTICES];
    unsigned int indices[MAX_INDICES];
    Texture textures[MAX_TEXTURES];

    Texture loaded_textures[MAX_TEXTURES];
    int num_loaded_textures = 0;

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position[0] = ai_mesh->mVertices[i].x;
        vertex.position[1] = ai_mesh->mVertices[i].y;
        vertex.position[2] = ai_mesh->mVertices[i].z;

        vertex.normals[0] = ai_mesh->mNormals[i].x;
        vertex.normals[1] = ai_mesh->mNormals[i].y;
        vertex.normals[2] = ai_mesh->mNormals[i].z;

        if (ai_mesh->mTextureCoords[0]) {
            vertex.tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertex.tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertex.tex_coords[0] = 0.0f;
            vertex.tex_coords[1] = 0.0f;
        }
        vertices[i] = vertex;
    }
    
    unsigned int index_offset = 0;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        const struct aiFace* face = &ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < face->mNumIndices; ++j) {
            indices[index_offset + j] = face->mIndices[j];
        }
        index_offset += face->mNumIndices;
    }    
    
    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
        unsigned int texture_count = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
        
        for (unsigned int i = 0; i < texture_count; i++) {                 
            struct aiString path;
            if (AI_SUCCESS == aiGetMaterialTexture(material, aiTextureType_DIFFUSE, i, &path, NULL, NULL, NULL, NULL, NULL, NULL)) {
                textures[i] = load_model_texture(path.data, "texture_diffuse1");
            }
        }        
    }
       
    processed_mesh = create_mesh(vertices, indices, textures, ai_mesh->mNumVertices, index_offset, 0);
    model->meshes[model->num_meshes++] = processed_mesh;
}

void draw_model(Model model, Shader shader)
{
    for (unsigned int i = 0; i < model.num_meshes; i++) {
        draw_mesh(model.meshes[i], shader);
    }
}

