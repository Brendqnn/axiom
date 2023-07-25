#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform struct Material {
    sampler2D texture_diffuse;
    float shininess;
} material;

void main()
{
    vec3 ambient = vec3(0.2, 0.2, 0.2);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(material.texture_diffuse, TexCoords).xyz;
    vec3 result = (ambient + diffuse) * vec3(1.0, 0.5, 0.2); // You can replace this color with the desired color.

    FragColor = vec4(result, 1.0);
}
