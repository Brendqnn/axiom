#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 1.0, 1.0); // Pink color (max red and blue, no green)
}



