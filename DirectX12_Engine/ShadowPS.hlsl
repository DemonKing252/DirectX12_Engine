#define NumPointLights 1 
struct DirectionalLight
{
    float3 Position;
    float FallOffStart;
    
    float3 Strength;
    float FallOffEnd;
    
    float3 Direction;
    float SpecularStrength;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix Model;
    matrix View;
    matrix Projection;
    
    float4 DiffuseAlbedo;
    float3 EyeWorldSpace;
    float FresnelFactor;
    
    DirectionalLight lights[NumPointLights];
}
struct Layout
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 fragPos : FRAG;
};
SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

float4 PSMain(Layout layout) : SV_TARGET
{
    float4 pixelColor = tex.Sample(samplerState, layout.texCoord);
    float4 ambientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    
    return pixelColor * ambientLight * DiffuseAlbedo; // Discard ALL LIGHT if we happen to be drawing a shadow
}