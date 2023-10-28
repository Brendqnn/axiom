#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform the vertex position
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    gl_Position = projection * view * worldPosition;

    fragTexCoord = inTexCoord;
    fragNormal = mat3(transpose(inverse(model))) * inNormal;
    fragWorldPos = vec3(worldPosition);
}
