#include "axmesh.h"


AXMesh ax_create_mesh(Vertex vertices[], unsigned int indices[], AXTexture textures[],
                 unsigned int num_vertices, unsigned int num_indices, unsigned int num_textures)
{
    AXMesh mesh = {0};

    mesh.num_vertices = num_vertices;
    mesh.num_indices = num_indices;
    mesh.num_textures = num_textures;

    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.textures = textures;

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

void ax_draw_mesh(AXMesh mesh, AXShader *shader)
{
    ax_use_shader(shader);

    for (unsigned int i = 0; i < mesh.num_textures; ++i)
    {
        char uniform_name[64];
        snprintf(uniform_name, sizeof(uniform_name), "material.texture%d", i);

        ax_set_shader_int(shader, uniform_name, i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

