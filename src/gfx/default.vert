#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords; // Pass the texture coordinates to the fragment shader

uniform mat4 tree_m;
uniform mat4 grass_m;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * tree_m * vec4(aPos, 1.0);
    TexCoords = aTexCoords; // Pass the texture coordinates to the fragment shader
}