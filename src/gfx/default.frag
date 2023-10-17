#version 330 core

in vec2 TexCoords; // Receive the texture coordinates from the vertex shader

out vec4 FragColor;

// Samplers and uniforms for the first model
uniform sampler2D texture_diffuse1; // Add a sampler for the diffuse texture of the first model
uniform sampler2D texture_specular1; // Add a sampler for the specular texture of the first model
uniform sampler2D texture_opacity1;  // Add a sampler for the opacity texture of the first model

// Samplers and uniforms for the second model
uniform sampler2D texture_diffuse2; // Add a sampler for the diffuse texture of the second model
uniform sampler2D texture_specular2; // Add a sampler for the specular texture of the second model
uniform sampler2D texture_opacity2;  // Add a sampler for the opacity texture of the second model

void main()
{
    // Sample the diffuse, specular, and opacity textures for both models
    vec4 diffuseColor1 = texture(texture_diffuse1, TexCoords);
    vec4 specularColor1 = texture(texture_specular1, TexCoords);
    float opacity1 = texture(texture_opacity1, TexCoords).r;

    vec4 diffuseColor2 = texture(texture_diffuse2, TexCoords);
    vec4 specularColor2 = texture(texture_specular2, TexCoords);
    float opacity2 = texture(texture_opacity2, TexCoords).r;

    // Combine the diffuse and specular colors and apply opacity for both models
    vec3 finalColor1 = diffuseColor1.rgb * specularColor1.rgb;
    vec3 finalColor2 = diffuseColor2.rgb * specularColor2.rgb;

    // Mix the colors of both models based on their opacity
    vec3 finalColor = mix(finalColor1, finalColor2, opacity2); // Adjust the mixing ratio as needed

    // Combine the opacity of both models
    float finalOpacity = opacity1 * opacity2;

    FragColor = vec4(finalColor, finalOpacity);
}
