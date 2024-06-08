#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lNormal;
layout (location = 2) in vec2 lTexCoords;
layout (location = 3) in float lFaceID;

layout (location = 16) out vec3 fWorldPos;
layout (location = 17) out float fFaceID;

layout(std140, binding = 0) uniform IrradianceConversion {
    mat4 projection;
    mat4 view;
};

void main() {
    fWorldPos = lPosition;
    fFaceID = lFaceID;
    gl_Position = projection * view * vec4(lPosition, 1.0);
}
