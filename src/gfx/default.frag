#version 330 core

in vec2 TexCoords;
in vec4 FragColor;  // Receive the output color from the vertex shader

out vec4 FinalColor;  // Pass the final fragment color to the output

uniform sampler2D texture_diffuse;  // Diffuse texture
uniform sampler2D texture_opacity;  // Opacity map

void main()
{
    vec4 diffuseColor = texture(texture_diffuse, TexCoords);
    float opacity = texture(texture_opacity, TexCoords).r;

    // Combine the colors using the opacity map
    vec4 finalColor = diffuseColor * opacity;

    FinalColor = finalColor;
}
