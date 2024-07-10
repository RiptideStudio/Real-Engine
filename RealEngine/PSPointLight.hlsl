static const float3 ambientColor = { 0, 0, 0.15f };
static const float3 materialColor = { 0.7f, 0.7f, 0.7f };
static const float3 diffuseColor = { 1.5f, 1.f, 1.f };
static const float diffuseIntensity = 0.1f;
static const float attConst = 5.f;
static const float attLin = 0.45f;
static const float attQuad = 0.025f;
static const float spread = 1000;
Texture2D tex;
SamplerState splr;

float4 main(float3 pos : Position, float3 n : Normal, float4 col : Color, float2 tc : TexCoord) : SV_TARGET
{
    float3 lightPos = { 0, 0, -5 };
    float3 worldPos = pos;
    float3 normal = normalize(n);
    float3 color = col.rgb;

    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float att = spread / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    const float3 diffuse = diffuseColor * diffuseIntensity * att * dot(dirToL, normal);
    

    return float4(saturate(diffuse + ambientColor) * color, 1.0f) * tex.Sample(splr, tc);
}
