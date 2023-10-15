#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "gfx/stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static Texture texture_from_file(const char* filename, const char* typeName);
static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model);
static Mesh* process_mesh(struct aiMesh *ai_mesh, const struct aiScene *scene);

static Texture texture_from_file(const char* filename, const char* typeName)
{
    Texture texture;
    memset(&texture, 0, sizeof(Texture));

    int width, height, numChannels;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numChannels, 0);
    if (!imageData) {
        fprintf(stderr, "Failed to load texture from file: %s\n", filename);
        return texture;
    }

    glGenTextures(1, &texture.id);

    GLenum format;
    if (numChannels == 1)
        format = GL_RED;
    else if (numChannels == 3)
        format = GL_RGB;
    else if (numChannels == 4)
        format = GL_RGBA;
    else {
        fprintf(stderr, "Invalid number of channels in texture: %d\n", numChannels);
        stbi_image_free(imageData);
        return texture;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(imageData);

    strncpy(texture.type, typeName, sizeof(texture.type) - 1);
    texture.type[sizeof(texture.type) - 1] = '\0';

    strncpy(texture.path, filename, sizeof(texture.path) - 1);
    texture.path[sizeof(texture.path) - 1] = '\0';

    return texture;
}

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
        process_node(node->mChildren[i], scene, model);
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

    Texture* textures = NULL;

    int num_textures = 0;

    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
        num_textures = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
        textures = malloc(num_textures * sizeof(Texture));

        for (unsigned int i = 0; i < num_textures; i++) {
            struct aiString path;
            if (AI_SUCCESS == aiGetMaterialTexture(material, aiTextureType_DIFFUSE, i, &path, NULL, NULL, NULL, NULL, NULL, NULL)) {
                textures[i] = texture_from_file(path.data, "texture_diffuse1");
            }
        }
    }

    Mesh *mesh = create_mesh(vertices, ai_mesh->mNumVertices, indices, ai_mesh->mNumFaces * 3, textures, num_textures);

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

void draw_model(Model *model, Shader shader)
{
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        draw_mesh(model->meshes[i], shader);
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
