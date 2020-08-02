#define NumPointLights 1 
struct PointLight
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
SamplerState samplerState : SAMPLER : register(s0);
Texture2D tex : TEXTURE : register(t0);

float4 PSMain(Layout layout) : SV_TARGET
{
    float4 pixelColor = tex.Sample(samplerState, layout.texCoord);
    
    float4 ambientLight = float4(0.3f, 0.3f, 0.3f, 1.0f);
    
    float attenuation = length(pLights[0].Position - layout.fragPos);
    
    float3 diffuse = float3(0, 0, 0), specular = float3(0, 0, 0);
    for (int i = 0; i < NumPointLights; i++)
    {
        if (attenuation <= pLights[i].FallOffEnd)
        {
            float3 norm = normalize(layout.normal);
            float3 lightDirection = normalize(pLights[0].Position - layout.fragPos);
            float3 diff = (dot(norm, lightDirection));
            diffuse += diff * pLights[i].Strength * ((pLights[i].FallOffEnd - attenuation) / (pLights[i].FallOffEnd - pLights[i].FallOffStart));;
    
            float3 viewDir = normalize(EyeWorldSpace.xyz - layout.fragPos);
            float3 reflectDir = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32) / (attenuation * attenuation);
            specular += pLights[i].SpecularStrength * spec * pLights[i].Strength.xyz;
    
        }
    }
    // If the light fall off end is too low it will be less than zero, which will make the surface black. We don't want that!
    float4 totalLight = ambientLight + float4(clamp(diffuse, 0.0f, 255.0f), 1.0f) + float4(clamp(specular, 0.0f, 255.0f), 1.0f);
    
    return (pixelColor * totalLight) * DiffuseAlbedo;
}