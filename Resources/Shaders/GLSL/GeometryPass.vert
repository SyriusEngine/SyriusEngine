#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec3 lTangent;
layout(location = 3) in vec2 lTexCoords;

out VSOut{
    vec4 worldPosition;
    vec2 texCoords;
    mat3 tbn;
} vs_out;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

layout(std140, binding = 1) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

struct TransformationData{
    mat4 modelMatrix;
    mat4 normalMatrix;
};

layout(std140, binding = 2) uniform ModelData {
    TransformationData transform;
};

void main() {
    mat3 truncNormalMatrix = mat3(transform.normalMatrix);
    vec3 N = normalize(truncNormalMatrix * lNormal);
    vec3 T = normalize(truncNormalMatrix * lTangent);
    vec3 B = cross(N, T);

    vs_out.texCoords = lTexCoords;
    vs_out.tbn = mat3(T, B, N);
    vs_out.worldPosition = transform.modelMatrix * vec4(lPosition, 1.0);;

    gl_Position = perspective * view * vs_out.worldPosition;

}