#include "mesh.h"

Mesh create_mesh(Vertex vertices[], unsigned int indices[], Texture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures)
{
    Mesh mesh = {0};

    mesh.num_vertices = num_vertices;
    mesh.num_indices = num_indices;
    mesh.num_textures = num_textures;
    
    mesh.vertices = malloc(num_vertices * sizeof(Vertex));
    mesh.indices = malloc(num_indices * sizeof(unsigned int));
    mesh.textures = malloc(num_textures * sizeof(Texture));

    memcpy(mesh.vertices, vertices, num_vertices * sizeof(Vertex));
    memcpy(mesh.indices, indices, num_indices * sizeof(unsigned int));
    memcpy(mesh.textures, textures, num_textures * sizeof(Texture));

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
    for (size_t i = 0; i < mesh.num_textures; i++) {
        Texture texture = mesh.textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        bind_texture(&texture);
    }
    
    glBindVertexArray(mesh.VAO); 
    glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
