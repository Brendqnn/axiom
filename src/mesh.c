#include "mesh.h"
#include <string.h>


Mesh create_mesh(Vertex *vertices, unsigned int vertex_count, unsigned int *indices, unsigned int index_count)
{
    Mesh mesh;

    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.vertex_count = vertex_count;
    mesh.index_count = index_count;

    mesh.vao = vao_create();
    vao_bind(mesh.vao);

    mesh.vbo = vbo_create(GL_ARRAY_BUFFER, false);
    vbo_buffer(mesh.vbo, vertices, 0, vertex_count * sizeof(Vertex));

    mesh.ebo = ebo_create();
    ebo_bind(mesh.ebo);
    
    vao_attr(mesh.vao, mesh.vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, Position));
    vao_attr(mesh.vao, mesh.vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, Normal));
    vao_attr(mesh.vao, mesh.vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (size_t)offsetof(Vertex, TexCoords));
    
    return mesh;
}

void destroy_mesh(Mesh *mesh)
{
    free(mesh->vertices);
    free(mesh->indices);
}

void draw_mesh(Mesh *mesh)
{
    vao_bind(mesh->vao);
    ebo_bind(mesh->ebo);

    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
}
