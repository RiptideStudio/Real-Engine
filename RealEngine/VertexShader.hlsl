cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 color : Color;
    float2 tex : TexCoord;
    
    float4 pos : SV_Position;
    
};

VSOut main(float3 pos : Position, float3 normal : Normal, float4 color : Color, float2 tex : TexCoord)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.f), model);
    vso.normal = mul(normal, (float3x3) model);
    vso.pos = mul(float4(pos, 1.f), modelViewProj);
    vso.color = color; // set the output parameter to the color parameter
    vso.tex = tex;
    return vso;
}
