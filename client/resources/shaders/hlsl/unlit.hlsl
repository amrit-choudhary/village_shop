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

    float4 outputPos = mul(float4(input.position, 1.0f), modelMatrix);
    outputPos = mul(outputPos, viewMatrix);
    outputPos = mul(outputPos, projectionMatrix);
    output.position = outputPos;
    output.normal = input.normal;
    output.uv = input.uv;
    output.color = float4(abs(input.normal), 1.0f);
    return output;
}

float4 PS(VertexOut input) : SV_Target
{
    return input.color;
}
