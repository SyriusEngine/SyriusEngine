#version 460

layout (location = 16) in vec3 fWorldPos;
layout (location = 17) in float fFaceID;

layout(location = 0) out vec4 left;
layout(location = 1) out vec4 right;
layout(location = 2) out vec4 top;
layout(location = 3) out vec4 bottom;
layout(location = 4) out vec4 front;
layout(location = 5) out vec4 back;

layout(binding = 0) uniform sampler2D texMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphere(vec3 v){
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 v = normalize(fWorldPos);
    vec2 uv = sampleSphere(v);
    vec4 color = texture(texMap, uv);
    uint attachmentIndex = uint(fFaceID);
    switch(attachmentIndex){
        case 0: left = color;   break;
        case 1: right = color;  break;
        case 2: top = color;    break;
        case 3: bottom = color; break;
        case 4: front = color;  break;
        case 5: back = color;   break;
    }
}