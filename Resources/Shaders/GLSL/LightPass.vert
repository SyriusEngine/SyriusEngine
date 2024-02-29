#version 460

layout(location = 0) in vec2 lPosition;
layout(location = 1) in vec2 lTexCoords;

out VSOut{
    vec2 texCoords;
    vec4 cameraPos;
} vsOut;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

void main() {
    gl_Position = vec4(lPosition, 0.0, 1.0);
    vsOut.texCoords = lTexCoords;
    vsOut.cameraPos = cameraPos;
}