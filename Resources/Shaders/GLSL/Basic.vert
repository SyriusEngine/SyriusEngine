#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec3 lTangent;
layout(location = 3) in vec2 lTexCoords;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

layout(std140, binding = 1) uniform ProjectionData {
    mat4 perspective;
};

struct TransformationData{
    mat4 modelMatrix;
    mat4 normalMatrix;
};

layout(std140, binding = 2) uniform ModelData {
    TransformationData transform[300];
};

void main() {
    vec4 worldPosition = transform[gl_InstanceID].modelMatrix * vec4(lPosition, 1.0);
    gl_Position = perspective * view * worldPosition;
}