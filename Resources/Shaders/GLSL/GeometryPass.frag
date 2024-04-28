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

layout(binding = 0) uniform sampler2D materialTex;

const float NUM_TEXTURES = 5.0;
const float TEXTURE_SIZE = 1.0f / NUM_TEXTURES;

void main() {
    vec2 textureOffset = vec2(fs_in.texCoords.x * TEXTURE_SIZE, fs_in.texCoords.y);

    vec4 tAlbedo = texture(materialTex, textureOffset);
    textureOffset.x += TEXTURE_SIZE;
    vec4 tNormal = texture(materialTex, textureOffset);
    textureOffset.x += TEXTURE_SIZE;
    vec4 tMetallic = texture(materialTex, textureOffset);
    textureOffset.x += TEXTURE_SIZE;
    vec4 tRoughness = texture(materialTex, textureOffset);
    textureOffset.x += TEXTURE_SIZE;
    vec4 tAO = texture(materialTex, textureOffset);

    vec3 N = normalize(fs_in.tbn * (tNormal.xyz * 2.0 - 1.0));

    gPosition = fs_in.worldPosition;
    gNormal = vec4(N, 1.0);
    gAlbedo = tAlbedo;
    gMetallicRoughnessAO = vec4(tMetallic.r, tRoughness.r, tAO.r, 1.0f);
}