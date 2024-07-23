#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 ambientColor;
uniform vec3 viewPos; 

void main() {
    vec3 lightDirection = normalize(-lightDir);  // invert lightDir for correct direction
    vec3 ambient = ambientColor;

    vec3 norm = normalize(Normal);
    if (texture(material.texture_normal1, TexCoord).r != 0.0) {
        norm = normalize(texture(material.texture_normal1, TexCoord).rgb * 2.0 - 1.0);
    }
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  // Specular shininess
    vec3 specular = specularStrength * spec * lightColor * texture(material.texture_specular1, TexCoord).rgb;

    vec4 textureColor = texture(material.texture_diffuse1, TexCoord);
    vec3 ambientAndDiffuse = textureColor.rgb * (ambient + diffuse);
    vec3 finalColor = ambientAndDiffuse + specular;

    FragColor = vec4(finalColor, 1.0);
}
