struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 color : Color;

    float4 pos : SV_Position;
};

static const float3 ambientColor = { 0.5, 0.45, 0.45 };
static const float3 materialColor = { 0.7f, 0.7f, 0.7f };
static const float3 diffuseColor = { 1.1f, 1.f, 2.f };
static const float diffuseIntensity = 0.5f;
static const float att = 0.5f;
static const float attLin = 0.0045f;
static const float attQuad = 0.00075f;
static const float spread = 12;

float4 main(VSOut input) : SV_TARGET
{
    float3 lightDir = { 0, -1, 0.5 };
    float3 worldPos = input.worldPos;
    float3 normal = normalize(input.normal);
    float3 color = input.color.rgb;

    const float3 dirToL = -lightDir;
    const float3 diffuse = diffuseColor * diffuseIntensity * att * dot(dirToL, normal);

    return float4(saturate(diffuse + ambientColor) * color, 1.0f);
}
