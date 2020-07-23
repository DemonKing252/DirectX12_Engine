cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix Model;
    matrix View;
    matrix Projection;    
    float4 DiffuseAlbedo;
}

struct Layout
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

float4 PSMain(Layout layout) : SV_TARGET
{
    return DiffuseAlbedo;
}