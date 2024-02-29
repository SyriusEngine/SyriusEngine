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
layout(binding = 2) uniform sampler2D metallicTex;
layout(binding = 3) uniform sampler2D roughnessTex;
layout(binding = 4) uniform sampler2D aoTex;

void main() {
    vec4 tAlbedo = texture(albedoTex, fs_in.texCoords);
    vec4 tNormal = texture(normalTex, fs_in.texCoords);
    vec4 tMetallic = texture(metallicTex, fs_in.texCoords);
    vec4 tRoughness = texture(roughnessTex, fs_in.texCoords);
    vec4 tAO = texture(aoTex, fs_in.texCoords);

    vec3 N = normalize(fs_in.tbn * (tNormal.xyz * 2.0 - 1.0));

    gPosition = fs_in.worldPosition;
    gNormal = vec4(N, 1.0);
    gAlbedo = tAlbedo;
    gMetallicRoughnessAO = vec4(tMetallic.r, tRoughness.r, tAO.r, 1.0f);
}