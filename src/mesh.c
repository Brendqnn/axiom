#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "mesh.h"

Mesh* create_mesh(Vertex* vertices, unsigned int* indices, unsigned int num_vertices, unsigned int num_indices) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) {
        printf("Failed to allocate memory for mesh\n");
        return NULL;
    }

    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;
    
    // Set up the VAO, VBO, and IBO
    vao_bind(mesh->vao);
    vbo_buffer(mesh->vbo, vertices, 0, sizeof(Vertex) * num_vertices);
    ibo_buffer(mesh->ibo, indices, sizeof(unsigned int) * num_indices, GL_STATIC_DRAW);
    
    // Configure vertex attributes
    vao_attr(mesh->vao, mesh->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vao_attr(mesh->vao, mesh->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    vao_unbind();
    
    return mesh;
}

void draw(Mesh* mesh) {
    // Bind VAO and IBO
    vao_bind(mesh->vao);
    ibo_bind(mesh->ibo);

    // Draw the mesh using the provided shader
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);

    // Unbind VAO and IBO
    vao_unbind();
    ibo_unbind();
}

void setup_mesh(Mesh* mesh) {
    
}

void destroy_mesh(Mesh* mesh) {
    if (mesh) {
        free(mesh->vertices);
        free(mesh->indices);
        // ...

        // Destroy VAO, VBO, and IBO
        vao_destroy(mesh->vao);
        vbo_destroy(mesh->vbo);
        ibo_destroy(mesh->ibo);

        free(mesh);
    }
}
