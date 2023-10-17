#include "mesh.h"

Mesh* create_mesh(Vertex* vertices, unsigned int num_vertices,
                  unsigned int* indices, unsigned int num_indices, Texture *textures, unsigned int num_textures)
{
    Mesh* mesh = malloc(sizeof(Mesh));

    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->textures = textures;
    
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;
    mesh->num_textures = num_textures;
    
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    return mesh;
}

void draw_mesh(Mesh* mesh, Shader shader)
{
    glBindVertexArray(mesh->VAO);

    for (unsigned int i = 0; i < mesh->num_textures; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void destroy_mesh(Mesh* mesh)
{
    if (mesh == NULL) {
        return;
    }
    free(mesh);
}
