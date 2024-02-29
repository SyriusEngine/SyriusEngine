#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec2 lTexCoord;
layout(location = 3) in float lFaceID;

layout(location = 16) out vec2 fTexCoord;
layout(location = 17) out float fFaceID;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

layout(std140, binding = 1) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

void main() {
    mat4 augmentedView = mat4(mat3(view));
    gl_Position = perspective * augmentedView * vec4(lPosition, 1.0);
    fTexCoord = lTexCoord;
    fFaceID = lFaceID;
}