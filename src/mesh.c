#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Mesh* createMesh(struct Vertex* vertices, GLuint* indices, struct Texture* textures, int numVertices, int numIndices, int numTextures)
{
    struct Mesh* mesh = (struct Mesh*)malloc(sizeof(struct Mesh));
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->textures = textures;
    mesh->numVertices = numVertices;
    mesh->numIndices = numIndices;
    mesh->numTextures = numTextures;

    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh(mesh);

    return mesh;
}

void Draw(struct Mesh* mesh, Shader shader)
{
    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    for (int i = 0; i < mesh->numTextures; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        char number[16];
        char name[100];
        strcpy(name, mesh->textures[i].type);

        if (strcmp(name, "texture_diffuse") == 0)
        {
            sprintf(number, "%d", diffuseNr++); // Convert int to string
        }
        else if (strcmp(name, "texture_specular") == 0)
        {
            sprintf(number, "%d", specularNr++); // Convert int to string
        }

        // Now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, strcat(name, number)), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }
 
    glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);
   
    //glUseProgram(shader.ID);

    //glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, (const GLfloat*)modelMatrix);

    // Draw mesh
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    for (int i = 0; i < mesh->numTextures; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


void setupMesh(struct Mesh* mesh)
{
    
    glGenVertexArrays(1, &(mesh->VAO));
    glGenBuffers(1, &(mesh->VBO));
    glGenBuffers(1, &(mesh->EBO));

    glBindVertexArray(mesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    
    glBufferData(GL_ARRAY_BUFFER, mesh->numVertices * sizeof(struct Vertex), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, Normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, TexCoords));

    glBindVertexArray(0);
}


