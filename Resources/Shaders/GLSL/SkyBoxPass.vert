#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lNormal;
layout (location = 2) in vec2 lTexCoords;
layout (location = 3) in float lFaceID;

layout (location = 16) out vec3 fWorldPos;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

layout(std140, binding = 1) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

void main() {
    fWorldPos = lPosition;

    mat4 rotView = mat4(mat3(view));
    vec4 clipped = perspective * rotView * vec4(lPosition, 1.0);

    gl_Position = clipped.xyww;
}