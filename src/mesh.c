#include "mesh.h"


Mesh* setup_mesh(Vertex* vertices, unsigned int* indices, unsigned int num_vertices, unsigned int num_indices) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) {
        fprintf(stderr, "Failed to allocate memory for Mesh.\n");
        return NULL;
    }

    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;

    mesh->vao = vao_create();
    vao_bind(mesh->vao);

    mesh->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    vbo_bind(mesh->vbo);

    vbo_buffer(mesh->vbo, vertices, num_vertices * sizeof(Vertex), GL_STATIC_DRAW);

    mesh->ibo = ibo_create();
    ibo_bind(mesh->ibo);

    ibo_buffer(mesh->ibo, indices, num_indices * sizeof(unsigned int), GL_STATIC_DRAW);

    vao_attr(mesh->vao, mesh->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, position));
    vao_attr(mesh->vao, mesh->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, normal));
    vao_attr(mesh->vao, mesh->vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, tex_coords));
    
    return mesh;
}

void draw(Mesh* mesh) {
    vao_bind(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
}

void mesh_destroy(Mesh* mesh) {
    if (!mesh)
        return;

    vao_destroy(mesh->vao);
    vbo_destroy(mesh->vbo);
    ibo_destroy(mesh->ibo);

    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}

