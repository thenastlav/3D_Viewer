#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform vec3 move;
uniform mat4 scale;
uniform mat4 transform;
uniform mat4 projection;

uniform vec3 uColor;
uniform float uPointSize;

void main()
{
    gl_PointSize = uPointSize;
    gl_Position = projection * transform * scale * vec4(aPos, 1.0) + vec4(move, 1.0);
    color = uColor;
}
