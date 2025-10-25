// Simple sprite shader for instanced rendering.

#include "structs.hlsl"

cbuffer CBPerPass : register(b0)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    LightDataAmbient ambientLightData;
    LightDataDirectional directionalLightData;
};

cbuffer CBPerObject : register(b1)
{
    float4x4 modelMatrix;
};

Texture2D tex : register(t0);

SamplerState texSampler : register(s0);

struct VertexIn
{
	float2 position  : POSITION;
    float2 uv : TEXCOORD0;
};

struct VertexOut
{
	float4 position  : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

    float4 outputPos = mul(float4(input.position, 0.0f, 1.0f), modelMatrix);
    outputPos = mul(outputPos, viewMatrix);
    outputPos = mul(outputPos, projectionMatrix);
    output.position = outputPos;
    output.uv = input.uv;

    return output;
}

float4 PS(VertexOut input) : SV_Target
{
    // flipping y in uv is directx specific.
    float2 flippedUV = float2(input.uv.x, 1.0f - input.uv.y);
    float4 texColor = tex.Sample(texSampler, flippedUV);
    if (texColor.a < 0.1f)
        clip(-1);
    
    return texColor;
}
