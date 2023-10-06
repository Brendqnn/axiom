#include "mesh.h"

Mesh* create_mesh(Vertex* vertices, unsigned int num_vertices, unsigned int* indices, unsigned int num_indices) {
    Mesh* mesh = malloc(sizeof(Mesh));

    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;

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

void draw_mesh(Mesh* mesh)
{
    glBindVertexArray(mesh->VAO);
    
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
}

void destroy_mesh(Mesh* mesh)
{
    if (mesh == NULL) {
        return;
    }
    free(mesh->vertices);
    free(mesh);
}

