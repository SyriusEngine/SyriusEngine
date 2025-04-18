#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec3 lTangent;
layout(location = 3) in vec2 lTexCoords;

void main() {
    gl_Position = vec4(lPosition, 1.0);
}