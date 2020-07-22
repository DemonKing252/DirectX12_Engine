cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix Model;
    matrix View;
    matrix Projection;
}
struct Layout
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

float4 PSMain(Layout layout) : SV_TARGET
{
    return float4(layout.normal, 1.0f);
}