#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;  // Add normal map texture
};

uniform Material material;

void main() {
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);

    vec3 normal = texture(material.texture_normal, TexCoord).xyz * 2.0 - 1.0;  // Fetch and normalize normal from normal map
    float spec = pow(max(dot(reflect(-lightDir, normal), normalize(FragPos - vec3(0.0, 0.0, 5.0))), 0.0), 32.0);  // Example specular calculation

    vec4 diffuseColor = texture(material.texture_diffuse, TexCoord);
    vec4 specularColor = texture(material.texture_specular, TexCoord);

    FragColor = vec4((diffuseColor.rgb * diff + specularColor.rgb * spec) * lightColor, 1.0);
}
