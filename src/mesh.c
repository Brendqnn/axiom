#include "mesh.h"

void setup_mesh(Mesh *mesh) {
    mesh->vao = vao_create();
    vao_bind(mesh->vao);

    mesh->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    vbo_bind(mesh->vbo);
    vbo_buffer(mesh->vbo, mesh->vertices, sizeof(Vertex) * mesh->vertex_count, GL_STATIC_DRAW);

    mesh->ebo = ebo_create();
    ebo_bind(mesh->ebo);
    ebo_buffer(mesh->ebo, mesh->indices, sizeof(unsigned int) * mesh->index_count, GL_STATIC_DRAW);

    vao_attr(mesh->vao, mesh->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, Position));
    vao_attr(mesh->vao, mesh->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, Normal));
    vao_attr(mesh->vao, mesh->vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, TexCoords));
}

Mesh *create_mesh(Vertex *vertices, unsigned int vertex_count, unsigned int *indices, unsigned int index_count) {
    Mesh *mesh = malloc(sizeof(Mesh));

    mesh->vertices = malloc(sizeof(Vertex) * vertex_count);
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * vertex_count);

    mesh->indices = malloc(sizeof(unsigned int) * index_count);
    memcpy(mesh->indices, indices, sizeof(unsigned int) * index_count);

    mesh->vertex_count = vertex_count;
    mesh->index_count = index_count;

    setup_mesh(mesh);

    return mesh;
}

void destroy_mesh(Mesh *mesh) {
    free(mesh->vertices);
    free(mesh->indices);

    free(mesh);
}

void draw_mesh(Mesh *mesh, Shader shader) {
    vao_bind(mesh->vao);
    ebo_bind(mesh->ebo);

    glUseProgram(shader.ID);

    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
}


