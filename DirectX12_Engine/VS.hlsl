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

Layout VSMain(float3 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    Layout layout;
    layout.position = mul(float4(position, 1.0f), World);
    layout.texCoord = texCoord;
    layout.normal = normal;
    
    return layout;
}