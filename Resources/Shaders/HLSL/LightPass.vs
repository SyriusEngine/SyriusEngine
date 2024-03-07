struct VS_IN{
    float2 position: Position;
    float2 texCoords: TexCoords;
};

struct VS_OUT{
    float4 position: SV_POSITION;
    float2 texCoords: TexCoords;
    float4 cameraPos: CameraPos;
};

cbuffer CameraData: register(b0){
    float4x4 view;
    float4 cameraPos;
};

VS_OUT main(VS_IN vsIn){
    VS_OUT vsOut;
    vsOut.position = float4(vsIn.position, 0.0, 1.0);
    vsOut.cameraPos = cameraPos;
    vsOut.texCoords = vsIn.texCoords;
    return vsOut;
}