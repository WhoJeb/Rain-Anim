#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 instanceOffset;

out vec3 vColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 pos = aPos + vec3(instanceOffset, 0.0);
    gl_Position = projection * view * vec4(pos, 1.0);
    vColor = aColor;
}

