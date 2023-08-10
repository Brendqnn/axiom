#include "mesh.h"


void setup_mesh(Mesh *mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->index_count, mesh->indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
}

Mesh *create_mesh(Vertex *vertices, unsigned int vertex_count, unsigned int *indices, unsigned int index_count)
{
    Mesh *mesh = malloc(sizeof(Mesh));
    
    mesh->vertices = vertices;
    mesh->indices = indices;

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

void draw_mesh(Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


