#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "gfx/stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static Texture texture_from_file(const char* filename, const char* typeName);
static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model);
static Mesh* process_mesh(struct aiMesh *ai_mesh, const struct aiScene *scene);

static Texture texture_from_file(const char* filename, const char* typeName) {
    Texture texture = {0};

    int width, height, numChannels;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numChannels, 0);
    if (!imageData) {
        fprintf(stderr, "Failed to load texture from file: %s\n", filename);
        return texture;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (numChannels == 1) ? GL_RED : (numChannels == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);
    
    strncpy(texture.type, typeName, sizeof(texture.type) - 1);
    texture.type[sizeof(texture.type) - 1] = '\0';
    strncpy(texture.path, filename, sizeof(texture.path) - 1);
    texture.path[sizeof(texture.path) - 1] = '\0';

    return texture;
}


static void process_node(struct aiNode *node, const struct aiScene *scene, Model *model)
{
    unsigned int totalMeshes = 0;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        if (process_mesh(mesh, scene)) {
            totalMeshes++;
        }
    }

    if (totalMeshes > 0) {
        model->meshes = realloc(model->meshes, (model->num_meshes + totalMeshes) * sizeof(Mesh*));
        if (!model->meshes) {
            fprintf(stderr, "Failed to allocate memory for meshes.\n");
            return;
        }
    }
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh *processed_mesh = process_mesh(mesh, scene);
        if (processed_mesh) {
            model->meshes[model->num_meshes++] = processed_mesh;
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, model);
    }
}

static Mesh* process_mesh(struct aiMesh* ai_mesh, const struct aiScene* scene) {
    if (!ai_mesh->mVertices || !ai_mesh->mNumVertices || !ai_mesh->mFaces || !ai_mesh->mNumFaces) {
        return NULL;
    }

    Vertex* vertices = malloc(ai_mesh->mNumVertices * sizeof(Vertex));
    unsigned int* indices = malloc(ai_mesh->mNumFaces * 3 * sizeof(unsigned int));

    if (!vertices || !indices) {
        free(vertices);
        free(indices);
        return NULL;
    }

    struct {
        enum aiTextureType type;
        const char* uniformName;
    } textureTypeMap[] = {
        {aiTextureType_DIFFUSE, "texture_diffuse1"},
        {aiTextureType_HEIGHT, "texture_specular1"},
        {aiTextureType_OPACITY, "texture_opacity1"},
    };

    Texture* textures = NULL;
    int num_textures = 0;

    if (ai_mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

        for (size_t i = 0; i < sizeof(textureTypeMap) / sizeof(textureTypeMap[0]); i++) {
            struct aiString path;
            enum aiTextureType textureType = textureTypeMap[i].type;

            if (AI_SUCCESS == aiGetMaterialTexture(material, textureType, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL)) {
                textures = realloc(textures, (num_textures + 1) * sizeof(Texture));
                textures[num_textures] = texture_from_file(path.data, textureTypeMap[i].uniformName);
                num_textures++;
            }
        }
    }

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        vertices[i].position[0] = ai_mesh->mVertices[i].x;
        vertices[i].position[1] = ai_mesh->mVertices[i].y;
        vertices[i].position[2] = ai_mesh->mVertices[i].z;
        
        vertices[i].normal[0] = ai_mesh->mNormals[i].x;
        vertices[i].normal[1] = ai_mesh->mNormals[i].y;
        vertices[i].normal[2] = ai_mesh->mNormals[i].z;     

        if (ai_mesh->mTextureCoords[0]) {
            vertices[i].tex_coords[0] = ai_mesh->mTextureCoords[0][i].x;
            vertices[i].tex_coords[1] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertices[i].tex_coords[0] = 0.0f;
            vertices[i].tex_coords[1] = 0.0f;
        }
    }

    unsigned int indexCount = 0;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
        struct aiFace face = ai_mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; k++) {
            indices[indexCount++] = face.mIndices[k];
        }
    }

    Mesh* mesh = create_mesh(vertices, ai_mesh->mNumVertices, indices, indexCount, textures, num_textures);

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

void destroy_model(Model *model, Shader shader)
{
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        destroy_mesh(model->meshes[i]);
    }
    shader_destroy(&shader);
    free(model->meshes);
    free(model);
}
