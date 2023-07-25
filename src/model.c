#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "gfx/stb_image.h"

// Assimp include
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_NUM_TEXTURES 8

static void processNode(const struct aiScene* scene, const struct aiNode* node, struct Model* model, const char* directory);
static struct Mesh processMesh(const struct aiMesh* mesh, const struct aiScene* scene, const char* directory);
static GLuint loadTexture(const char* path, const char* directory);

struct Model* loadModel(const char* filePath)
{
    const struct aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene)
    {
        fprintf(stderr, "Error: Unable to load model '%s'\n", filePath);
        return NULL;
    }

    struct Model* model = (struct Model*)malloc(sizeof(struct Model));
    model->numMeshes = scene->mNumMeshes;
    model->meshes = (struct Mesh*)malloc(model->numMeshes * sizeof(struct Mesh));

    const char* directory = strrchr(filePath, '/');
    if (!directory)
        directory = strrchr(filePath, '\\');
    if (!directory)
        directory = filePath;
    else
        directory++;

    processNode(scene, scene->mRootNode, model, directory);
    aiReleaseImport(scene);

    return model;
}

void drawModel(struct Model* model, Shader shader)
{
    for (int i = 0; i < model->numMeshes; i++)
    {
        Draw(&(model->meshes[i]), shader);
    }
}

void destroyModel(struct Model* model)
{
    if (!model)
        return;

    for (int i = 0; i < model->numMeshes; i++)
    {
        free(model->meshes[i].vertices);
        free(model->meshes[i].indices);
        for (int j = 0; j < model->meshes[i].numTextures; j++)
        {
            glDeleteTextures(1, &(model->meshes[i].textures[j].id));
        }
    }
    free(model->meshes);
    free(model);
}

static void processNode(const struct aiScene* scene, const struct aiNode* node, struct Model* model, const char* directory)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes[i] = processMesh(mesh, scene, directory);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(scene, node->mChildren[i], model, directory);
    }
}

static struct Mesh processMesh(const struct aiMesh* mesh, const struct aiScene* scene, const char* directory)
{
    struct Vertex* vertices = (struct Vertex*)malloc(mesh->mNumVertices * sizeof(struct Vertex));
    GLuint* indices = (GLuint*)malloc(mesh->mNumFaces * 3 * sizeof(GLuint));
    struct Texture* textures = (struct Texture*)malloc(MAX_NUM_TEXTURES * sizeof(struct Texture));

    int numVertices = mesh->mNumVertices;
    int numIndices = mesh->mNumFaces * 3;
    int numTextures = 0;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Position
        vertices[i].Position[0] = mesh->mVertices[i].x;
        vertices[i].Position[1] = mesh->mVertices[i].y;
        vertices[i].Position[2] = mesh->mVertices[i].z;

        // Normals
        vertices[i].Normal[0] = mesh->mNormals[i].x;
        vertices[i].Normal[1] = mesh->mNormals[i].y;
        vertices[i].Normal[2] = mesh->mNormals[i].z;

        // Texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            vertices[i].TexCoords[0] = mesh->mTextureCoords[0][i].x;
            vertices[i].TexCoords[1] = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertices[i].TexCoords[0] = 0.0f;
            vertices[i].TexCoords[1] = 0.0f;
        }
    }

    // Process indices
    int indexCount = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const struct aiFace* face = &(mesh->mFaces[i]);
        for (unsigned int j = 0; j < face->mNumIndices; j++)
        {
            indices[indexCount++] = face->mIndices[j];
        }
    }

    // Process materials/textures
    if (mesh->mMaterialIndex >= 0)
    {
        const struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Diffuse textures
        struct Texture texture;
        struct aiString texturePath;
        if (aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &texturePath, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
            strcpy(texture.type, "texture_diffuse");
            strcpy(texture.path, directory);
            strcat(texture.path, "/");
            strcat(texture.path, texturePath.data);
            textures[numTextures++] = texture;
        }

        for (int i = 0; i < numTextures; i++)
        {
            textures[i].id = loadTexture(textures[i].path, directory);
        }
    }

    struct Mesh meshData;
    meshData.vertices = vertices;
    meshData.indices = indices;
    meshData.textures = textures;
    meshData.numVertices = numVertices;
    meshData.numIndices = numIndices;
    meshData.numTextures = numTextures;
    setupMesh(&meshData);

    return meshData;
}

static GLuint loadTexture(const char* path, const char* directory)
{
    char fullPath[200];
    strcpy(fullPath, directory);
    strcat(fullPath, "/");
    strcat(fullPath, path);

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(fullPath, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        fprintf(stderr, "Error: Texture loading failed for '%s'\n", fullPath);
        stbi_image_free(data);
    }

    return textureID;
}



