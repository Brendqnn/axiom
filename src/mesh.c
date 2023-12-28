#include "mesh.h"

Mesh create_mesh(Vertex vertices[], unsigned int indices[], Texture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures)
{
     Mesh mesh;

     if (num_vertices <= MAX_VERTICES) {
        memcpy(mesh.vertices, vertices, num_vertices * sizeof(Vertex));
        mesh.num_vertices = num_vertices;
    } else {
         fprintf(stderr, "Error: Number of vertices exceeds the maximum allowed size\n");
    }

    if (num_indices <= MAX_INDICES) {
        memcpy(mesh.indices, indices, num_indices * sizeof(unsigned int));
        mesh.num_indices = num_indices;
    } else {
        fprintf(stderr, "Error: Number of indices exceeds the maximum allowed size\n");
    }

    if (num_textures <= MAX_TEXTURES) {
        memcpy(mesh.textures, textures, num_textures * sizeof(Texture));
        mesh.num_textures = num_textures;
    } else {
        fprintf(stderr, "Error: Number of textures exceeds the maximum allowed size\n");
    }

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    
    return mesh;
}

void draw_mesh(Mesh mesh, Shader shader)
{
    glBindVertexArray(mesh.VAO);

    glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}
