#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

// Texture sampler
uniform sampler2D texture_diffuse1;

void main()
{
    // Sample diffuse texture
    vec3 diffuseColor = texture(texture_diffuse1, fragTexCoord).rgb;

    // Set the final color
    fragColor = vec4(diffuseColor, 1.0);
}
