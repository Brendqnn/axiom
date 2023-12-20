#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 diffuseColor = texture(texture_diffuse1, fragTexCoord).rgb;

    fragColor = vec4(diffuseColor, 1.0);
}