#version 460

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec3 lTangent;
layout(location = 3) in vec2 lTexCoords;

const vec2 transforms[4] = {
    vec2(-0.5, -0.5),
    vec2(0.5, -0.5),
    vec2(-0.5, 0.5),
    vec2(0.5, 0.5)
};

void main() {
    vec4 position = vec4(lPosition, 1.0);
    vec2 offset = transforms[gl_InstanceID];
    position.x += offset.x;
    position.y += offset.y;
    gl_Position = position;
}