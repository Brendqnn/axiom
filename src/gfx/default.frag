#version 330 core

in vec2 TexCoords; // Receive the texture coordinates from the vertex shader

out vec4 FragColor;

uniform sampler2D texture_diffuse1; // Add a sampler for your diffuse texture

void main()
{
    vec4 textureColor = texture(texture_diffuse1, TexCoords); // Sample the diffuse texture
    FragColor = textureColor; // Use the texture color as the final fragment color
}

