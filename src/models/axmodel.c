#include "axmodel.h"
#include <pthread.h>
#include <immintrin.h>


typedef struct {
    enum aiTextureType type;
    const char* type_name;
} TextureInfo;

unsigned int count_all_textures(const struct aiScene* scene, enum aiTextureType texture_type)
{
    unsigned int count = 0;
    
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        struct aiMaterial* material = scene->mMaterials[i];
        count += aiGetMaterialTextureCount(material, texture_type);
    }
    
    return count;
}

AXTexture* load_material_textures(struct aiMaterial* material, enum aiTextureType texture_type, const char* texture_type_name)
{
    unsigned int texture_count = aiGetMaterialTextureCount(material, texture_type);
    if (texture_count == 0) {
        return NULL;
    }

    AXTexture* textures = (AXTexture*)malloc(texture_count * sizeof(AXTexture));
    for (unsigned int i = 0; i < texture_count; ++i) {
        struct aiString texture_path;
        if (aiGetMaterialTexture(material, texture_type, i, &texture_path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            textures[i] = ax_load_model_texture(texture_path.data, texture_type_name);
            textures[i].count = texture_count;
        }
    }
    
    return textures;
}

void add_textures(AXTexture* textures, unsigned int texture_index, TextureInfo info, struct aiMaterial* material)
{
    AXTexture* texture_array = load_material_textures(material, info.type, info.type_name);
    if (texture_array) {
        for (unsigned int i = 0; i < texture_array->count; ++i) {
            textures[texture_index++] = texture_array[i];
        }
        free(texture_array);
    }
}

AXModel ax_load_model(const char* model_path)
{
    AXModel model = {0};

    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        return model;
    }

    ax_process_node(scene->mRootNode, scene, &model);

    aiReleaseImport(scene);
    return model;
}

void ax_process_node(const struct aiNode* node, const struct aiScene* scene, AXModel *model)
{    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const struct aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        ax_process_mesh(aiMesh, scene, model);
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ax_process_node(node->mChildren[i], scene, model);
    }
}

/* void ax_process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, AXModel* model) */
/* { */
/*     unsigned int num_vertices = ai_mesh->mNumVertices; */
/*     unsigned int num_indices = 0; */

/*     Vertex* vertices = (Vertex*)malloc(num_vertices * sizeof(Vertex)); */
/*     for (unsigned int i = 0; i < num_vertices; ++i) { */
/*         vertices[i].position[0] = ai_mesh->mVertices[i].x; */
/*         vertices[i].position[1] = ai_mesh->mVertices[i].y; */
/*         vertices[i].position[2] = ai_mesh->mVertices[i].z; */

/*         vertices[i].normals[0] = ai_mesh->mNormals[i].x; */
/*         vertices[i].normals[1] = ai_mesh->mNormals[i].y; */
/*         vertices[i].normals[2] = ai_mesh->mNormals[i].z; */

/*         if (ai_mesh->mTextureCoords[0]) { */
/*             vertices[i].tex_coords[0] = ai_mesh->mTextureCoords[0][i].x; */
/*             vertices[i].tex_coords[1] = ai_mesh->mTextureCoords[0][i].y; */
/*         } else { */
/*             vertices[i].tex_coords[0] = 0.0f; */
/*             vertices[i].tex_coords[1] = 0.0f; */
/*         } */
/*     } */

/*     for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) { */
/*         num_indices += ai_mesh->mFaces[i].mNumIndices; */
/*     } */

/*     unsigned int* indices = (unsigned int*)malloc(num_indices * sizeof(unsigned int)); */
/*     unsigned int idx = 0; */
/*     for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) { */
/*         struct aiFace face = ai_mesh->mFaces[i]; */
/*         for (unsigned int k = 0; k < face.mNumIndices; ++k) { */
/*             indices[idx++] = face.mIndices[k]; */
/*         } */
/*     } */

/*     TextureInfo texture_infos[] = { */
/*         {aiTextureType_DIFFUSE, "texture_diffuse1"}, */
/*         {aiTextureType_SPECULAR, "texture_specular1"}, */
/*         {aiTextureType_NORMALS, "texture_normal1"} */
/*     }; */

/*     unsigned int num_textures = 0; */
/*     for (unsigned int i = 0; i < sizeof(texture_infos) / sizeof(texture_infos[0]); ++i) { */
/*         num_textures += aiGetMaterialTextureCount(scene->mMaterials[ai_mesh->mMaterialIndex], texture_infos[i].type); */
/*     } */

/*     AXTexture* textures = (AXTexture*)malloc(num_textures * sizeof(AXTexture)); */
/*     if (!textures) { */
/*         fprintf(stderr, "Memory allocation failed for textures\n"); */
/*         free(vertices); */
/*         free(indices); */
/*         return; */
/*     } */

/*     unsigned int texture_index = 0; */

/*     for (unsigned int i = 0; i < sizeof(texture_infos) / sizeof(texture_infos[0]); ++i) { */
/*         add_textures(textures, texture_index, texture_infos[i], scene->mMaterials[ai_mesh->mMaterialIndex]); */
/*     } */
    
/*     if (scene->mNumAnimations > 0) { */
/*         printf("hello\n"); */
/*     } */

/*     AXMesh processed_mesh = ax_create_mesh(vertices, indices, textures, num_vertices, num_indices, num_textures); */
/*     model->meshes[model->num_meshes++] = processed_mesh; */

/*     free(vertices); */
/*     free(indices); */
/* } */

void ax_process_mesh(const struct aiMesh* ai_mesh, const struct aiScene* scene, AXModel* model)
{
    unsigned int num_vertices = ai_mesh->mNumVertices;
    unsigned int num_indices = 0;

    Vertex* vertices = (Vertex*)malloc(num_vertices * sizeof(Vertex));
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

    AXTexture* textures = NULL;
    unsigned int num_textures = 0;

    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
        
        AXTexture* diffuse_textures = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse1");
        AXTexture* specular_textures = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular1");
        AXTexture* normal_textures = load_material_textures(material, aiTextureType_NORMALS, "texture_normal1");

        num_textures = (diffuse_textures ? diffuse_textures->count : 0) +
            (specular_textures ? specular_textures->count : 0) +
            (normal_textures ? normal_textures->count : 0);

        textures = (AXTexture*)malloc(num_textures * sizeof(AXTexture));
        unsigned int texture_index = 0;

        if (diffuse_textures) {
            for (unsigned int i = 0; i < diffuse_textures->count; ++i) {
                textures[texture_index++] = diffuse_textures[i];
            }
        }
        if (specular_textures) {
            for (unsigned int i = 0; i < specular_textures->count; ++i) {
                textures[texture_index++] = specular_textures[i];
            }
        }
        if (normal_textures) {
            for (unsigned int i = 0; i < normal_textures->count; ++i) {
                textures[texture_index++] = normal_textures[i];
            }
        }

        free(diffuse_textures);
        free(specular_textures);
        free(normal_textures);
    }
    
    if (scene->mNumAnimations > 0) {
        printf("hello\n");
    }

    AXMesh processed_mesh = ax_create_mesh(vertices, indices, textures, num_vertices, num_indices, num_textures);
    model->meshes[model->num_meshes++] = processed_mesh;

    free(vertices);
    free(indices);
}

void ax_draw_model(AXModel model, AXShader* shader)
{
    for (unsigned int i = 0; i < model.num_meshes; i++) {
        ax_draw_mesh(model.meshes[i], shader);
    }
}

void ax_draw_model_instances(AXModel model, AXShader* shader, unsigned int instance_count)
{
    for (int i = 0; i < model.num_meshes; ++i) {
        ax_draw_mesh_instances(model.meshes[i], shader, instance_count);
    }
}

void ax_free_model(AXModel* model)
{
    for (unsigned int i = 0; i < model->num_meshes; ++i) {
        ax_free_mesh(model->meshes[i]);
    }
}
