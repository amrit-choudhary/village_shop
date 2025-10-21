// Simple sprite shader for instanced rendering.

cbuffer cbPerObject : register(b0)
{
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float rotation;
};

struct VertexIn
{
	float3 position  : POSITION;
    float3 normal    : NORMAL;
    float4 uv : TEXCOORD0;
};

struct VertexOut
{
	float4 position  : SV_POSITION;
    float3 normal    : NORMAL;
    float4 uv : TEXCOORD0;
    float4 color : COLOR0;
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

    float c = cos(rotation);
    float s = sin(rotation);
    float scale = 0.4f; // hard-coded scale

    float4x4 rotXYScale =
    {
        c * scale,   s * s * scale,   s * c * scale,   0,
        0,           c * scale,      -s * scale,       0,
       -s * scale,   s * c * scale,   c * c * scale,   0,
        0,           0,               0.5f,            1
    };

    float4x4 aspectMat =
    {
        1.33f, 0,      0, 0,
        0,      1,      0, 0,
        0,      0,      1, 0,
        0,      0,      0, 1
    };

    float4 rotatedPos = mul(float4(input.position, 1.0f), rotXYScale);
    rotatedPos = mul(rotatedPos, aspectMat);
    output.position = rotatedPos;
    output.normal = input.normal;
    output.uv = input.uv;
    output.color = float4(abs(input.normal), 1.0f);
    return output;
}

float4 PS(VertexOut input) : SV_Target
{
    return input.color;
}
