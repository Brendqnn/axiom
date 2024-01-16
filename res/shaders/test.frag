#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
    // Output a fixed blue color
    fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
