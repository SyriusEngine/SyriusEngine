#version 460

layout(location = 16) in vec2 fTexCoord;
layout(location = 17) in float fFaceID;

layout (location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D right;
layout(binding = 1) uniform sampler2D left;
layout(binding = 2) uniform sampler2D top;
layout(binding = 3) uniform sampler2D bottom;
layout(binding = 4) uniform sampler2D front;
layout(binding = 5) uniform sampler2D back;

void main() {
    uint faceID = uint(fFaceID);
    switch(faceID){
        case 0: fragColor = texture(left, fTexCoord); break;
        case 1: fragColor = texture(right, fTexCoord); break;
        case 2: fragColor = texture(top, fTexCoord); break;
        case 3: fragColor = texture(bottom, fTexCoord); break;
        case 4: fragColor = texture(front, fTexCoord); break;
        case 5: fragColor = texture(back, fTexCoord); break;
    }

}