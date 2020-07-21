cbuffer ConstantBuffer : register(b0)
{
    float4 Color;
}
float4 PSMain() : SV_TARGET
{
    return Color;
}