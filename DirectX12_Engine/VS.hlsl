#define NumPointLights 1 
struct PointLight
{
    float3 Position;
    float FallOffStart;
    float3 Strength;
    float FallOffEnd;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix Model;
    matrix View;
    matrix Projection;
    
    float4 DiffuseAlbedo;
    
    float4 EyeWorldSpace;
    
    PointLight pLights[NumPointLights];
}

struct Layout
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 fragPos : FRAG;
};

Layout VSMain(float3 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    Layout layout;
    layout.position = mul(float4(position, 1.0f), World);
    layout.texCoord = texCoord;
    layout.normal = mul(normal, (float3x3)Model);
    layout.fragPos = mul(float4(position, 1.0f), Model).xyz;
    
    return layout;
}