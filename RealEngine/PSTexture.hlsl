static const float3 ambientColor = { 0.45, 0.45, 0.45 };
static const float3 diffuseColor = { 0.5f, 0.5f, 1.5f };
static const float4 post = { 1.25f,1,1,1 };
static const float diffuseIntensity = 1.f;
static const float att = 0.5f;
static const float attLin = 0.0045f;
static const float attQuad = 0.00075f;
static const float spread = 12;

Texture2D tex;
SamplerState splr;

float4 main(float3 pos : Position, float3 n : Normal, float4 col : Color, float2 tc : TexCoord) : SV_TARGET
{
    float3 lightDir = { 0.33, -1, 1 };
    float3 worldPos = pos;
    float3 normal = normalize(n);
    float3 color = col.rgb;

    const float3 dirToL = -lightDir;
    const float3 diffuse = diffuseColor * diffuseIntensity * att * dot(dirToL, normal);

    return (float4(saturate(diffuse + ambientColor), 1.0f) * tex.Sample(splr, tc)) * post;
}
