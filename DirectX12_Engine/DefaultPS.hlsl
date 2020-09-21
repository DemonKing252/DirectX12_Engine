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

	float4 ambientLight;
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
    
    float attenuation = length(lights[0].Position - layout.fragPos);
    
    float3 diffuse = float3(0, 0, 0), specular = float3(0, 0, 0);
    for (int i = 0; i < NumPointLights; i++)
    {
        if (attenuation <= lights[i].FallOffEnd)
        {
            float3 norm = normalize(layout.normal);
            float3 lightDirection = normalize(-lights[0].Direction);
            float3 diff = (dot(norm, lightDirection));
            diffuse += diff * lights[i].Strength;
            
            float3 viewDir = normalize(EyeWorldSpace - layout.fragPos);
            float3 reflectDir = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
            specular += lights[i].SpecularStrength * FresnelFactor * spec * lights[i].Strength.xyz;
    
        }
    }
    // If the light fall off end is too low it will be less than zero, which will make the surface black. We don't want that!
    float3 totalLight = ambientLight.xyz + clamp(diffuse, 0.0f, 255.0f) + clamp(specular, 0.0f, 255.0f);
    
    return pixelColor * float4(totalLight, 1.0f) * DiffuseAlbedo;
}