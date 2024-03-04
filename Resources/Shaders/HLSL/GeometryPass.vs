struct VS_IN{
    float3 lPosition: Position;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoords: TexCoords;
};

struct VS_OUT{
    float4 position: SV_Position;
    float4 worldPosition: WorldPosition;
    float2 texCoords: TexCoords;
    float3x3 TBN: TBNMatrix;
};

cbuffer CameraData: register(b0){
    float4x4 view;
    float4 cameraPos;
};

cbuffer ProjectionData: register(b1){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
}

struct TransformationData{
    float4x4 modelMatrix;
    float4x4 normalMatrix;
};

cbuffer ModelData: register(b2){
    TransformationData transform;
}

VS_OUT main(VS_IN vsIn){
    matrix<float, 3, 3> truncNormalMatrix = (float3x3) transform.normalMatrix;
    float3 N = normalize(mul(truncNormalMatrix, vsIn.lNormal));
    float3 T = normalize(mul(truncNormalMatrix, vsIn.lTangent));
    float3 B = cross(N, T);

    VS_OUT vsOut;
    vsOut.worldPosition = mul(transform.modelMatrix, float4(vsIn.lPosition, 1.0));
    vsOut.TBN = float3x3(T, B, N);
    vsOut.texCoords = vsIn.lTexCoords;

    float4 viewPos = mul(view, vsOut.worldPosition);
    vsOut.position = mul(perspective, viewPos);

    return vsOut;
}

