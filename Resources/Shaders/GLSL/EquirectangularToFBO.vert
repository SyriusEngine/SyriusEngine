#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec2 lTexCoord;
layout(location = 3) in float lFaceID;

layout(location = 16) out vec3 fWorldPosition;
layout(location = 17) out float fFaceID;

layout(std140, binding = 0) uniform ProjectionData {
    mat4 perspective;
    mat4 view;
};

void main() {
    fWorldPosition = lPosition;
    fFaceID = lFaceID;
    gl_Position = perspective * view * vec4(lPosition, 1.0);

}