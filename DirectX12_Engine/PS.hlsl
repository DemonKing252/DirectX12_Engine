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
SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

float4 PSMain(Layout layout) : SV_TARGET
{
    float pixelColor = tex.Sample(samplerState, layout.texCoord);
    
    return pixelColor * DiffuseAlbedo;
}