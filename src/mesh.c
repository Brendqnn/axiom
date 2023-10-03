#include "mesh.h"

Mesh* create_mesh(Vertex* vertices, unsigned int num_vertices, GLuint* indices, unsigned int num_indices) {
    Mesh* mesh = malloc(sizeof(Mesh));

    if (mesh == NULL) {
        return NULL;
    }

    // Allocate memory for the vertices and copy the data
    mesh->vertices = malloc(sizeof(Vertex) * num_vertices);
    if (mesh->vertices == NULL) {
        free(mesh);
        return NULL;
    }
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * num_vertices);
    mesh->num_vertices = num_vertices;

    // Allocate memory for the indices and copy the data
    mesh->indices = malloc(sizeof(GLuint) * num_indices);
    if (mesh->indices == NULL) {
        free(mesh->vertices); // Clean up previously allocated memory
        free(mesh);
        return NULL;
    }
    memcpy(mesh->indices, indices, sizeof(GLuint) * num_indices);
    mesh->num_indices = num_indices;
    
    mesh->vao = vao_create();
    vao_bind(mesh->vao);

    mesh->vbo = vbo_create(GL_ARRAY_BUFFER, GL_FALSE);
    vbo_buffer(mesh->vbo, vertices, sizeof(Vertex) * num_vertices, 0);

    mesh->ebo = ebo_create();
    ebo_buffer(mesh->ebo, indices, sizeof(GLuint) * num_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);
   
    return mesh;
}

void draw_mesh(Mesh* mesh)
{
    vao_bind(mesh->vao);

    ebo_bind(mesh->ebo);
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

