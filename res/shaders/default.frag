#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

void main() {
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(vec3(0.0, 1.0, 1.0));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);

    vec4 textureColor = texture(material.texture_diffuse1, TexCoord);

    FragColor = vec4(textureColor.rgb * diff * lightColor, 1.0);
}
