#version 460

in VSOut{
    vec4 worldPosition;
    vec2 texCoords;
    mat3 tbn;
} fs_in;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMetallicRoughnessAO;

layout(binding = 0) uniform sampler2D albedoTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D mraoTex;

void main() {
    vec4 albedo = texture(albedoTex, fs_in.texCoords);
    vec4 normalTexel = texture(normalTex, fs_in.texCoords);
    vec4 mrao = texture(mraoTex, fs_in.texCoords);

    vec3 transformedNormal = normalTexel.xyz * 2.0 - 1.0;
    vec3 normal = normalize(fs_in.tbn * transformedNormal);

    gPosition = fs_in.worldPosition;
    gNormal = vec4(normal, 1.0);
    gAlbedo = albedo;
    gMetallicRoughnessAO = mrao;
}