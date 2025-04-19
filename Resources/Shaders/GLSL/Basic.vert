#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec3 lTangent;
layout(location = 3) in vec2 lTexCoords;

struct TransformationData{
    mat4 modelMatrix;
    mat4 normalMatrix;
};


layout(std140, binding = 2) uniform ModelData {
    TransformationData transform[300];
};

void main() {
    gl_Position = transform[gl_InstanceID].modelMatrix * vec4(lPosition, 1.0);
}