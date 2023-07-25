#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include "shader.h"

struct Vertex
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    char type[100];
    char path[100];
};

struct Mesh
{
    struct Vertex* vertices;
    GLuint* indices;
    struct Texture* textures;
    int numVertices;
    int numIndices;
    int numTextures;
    GLuint VAO, VBO, EBO;
};

struct Mesh* createMesh(struct Vertex* vertices, GLuint* indices, struct Texture* textures, int numVertices, int numIndices, int numTextures);
void Draw(struct Mesh* mesh, Shader shader);
void setupMesh(struct Mesh* mesh);

#endif // MESH_H



