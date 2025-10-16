// Simple sprite shader for instanced rendering.

struct VertexIn
{
	float3 position  : POSITION;
    float4 uv : TEXCOORD0;
};

struct VertexOut
{
	float4 position  : SV_POSITION;
    float4 uv : TEXCOORD0;
    float4 color : COLOR0;
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

    output.position = float4(input.position, 1.0f);
    output.uv = input.uv;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return output;
}

float4 PS(VertexOut input) : SV_Target
{
    return input.color;
}
