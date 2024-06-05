#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    // Add more if needed
};

uniform Material material;

void main() {
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(vec3(0.0, 1.0, 1.0));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);

    vec4 textureColor1 = texture(material.texture_diffuse1, TexCoord);
    vec4 textureColor2 = texture(material.texture_diffuse2, TexCoord);

    vec4 finalColor = mix(textureColor1, textureColor2, 0.5); // Mix the textures
    FragColor = vec4(finalColor.rgb * diff * lightColor, 1.0);
}
