#version 330 core

in vec2 TexCoords; // Receive the texture coordinates from the vertex shader

out vec4 FragColor;

uniform sampler2D textureSampler; // The texture sampler

void main()
{
    FragColor = texture(textureSampler, TexCoords); // Sample the texture at TexCoords
}

