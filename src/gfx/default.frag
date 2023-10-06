#version 330 core

in vec2 TexCoords; // Receive the texture coordinates from the vertex shader

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color (R=1, G=0, B=0, A=1) for the entire model
}

