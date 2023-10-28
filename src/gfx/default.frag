#version 330 core

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 fragColor;

// Texture samplers
uniform sampler2D texture_diffuse1; // Map_Kd
uniform sampler2D texture_normal1;  // Map_Bump or normal map
uniform sampler2D texture_opacity1; // map_d or opacity map

void main()
{
    // Sample the diffuse texture (color texture)
    vec3 diffuseColor = texture(texture_diffuse1, fragTexCoord).rgb;

    // Sample the normal map (bump map)
    vec3 normalMap = texture(texture_normal1, fragTexCoord).xyz * 2.0 - 1.0;
    vec3 normal = normalize(fragNormal + normalMap);

    // Sample the opacity map (alpha channel)
    float opacity = texture(texture_opacity1, fragTexCoord).r;

    // Calculate the lighting (you may have more complex lighting calculations)
    vec3 ambientColor = vec3(1.0); // Modify ambient color if needed
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction
    float diff = max(dot(normal, lightDir), 0.0);

    // Combine the colors and opacity
    vec3 resultColor = diffuseColor;

    fragColor = vec4(resultColor, opacity);
}
