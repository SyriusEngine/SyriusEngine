#version 460

layout (location = 16) in vec3 fWorldPos;

layout (location = 0) out vec4 lColor;

uniform samplerCube environmentMap;

void main() {
    vec3 envColor = texture(environmentMap, fWorldPos).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    lColor = vec4(envColor, 1.0);
}