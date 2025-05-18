#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float2 position [[attribute(0)]];
    float2 uv       [[attribute(1)]];
};

struct VertexOut {
    float4 position [[position]];
    float2 uv;
    float4 tint;
};

vertex VertexOut vertexMain(VertexIn in [[stage_in]], 
                            constant float4x4& modelMatrix      [[buffer(1)]],
                            constant float4x4& projectionMatrix [[buffer(2)]],
                            constant float4& tint               [[buffer(3)]]) {

    VertexOut out;
    // out.position = projectionMatrix * viewMatrix * modelMatrix * float4(in.position, 1.0f);
    out.position = float4(in.position, 0.0f, 1.0f);
    out.uv = float2(in.uv.x, 1.0f - in.uv.y);
    out.tint = tint;
    return out;
}

fragment half4 fragmentMain(VertexOut in [[stage_in]],
                            texture2d<half, access::sample> tex             [[texture(0)]], 
                            sampler textureSampler                          [[sampler(0)]]) {

    half4 texel =  tex.sample(textureSampler, in.uv);
    if(texel.a < 0.1f){
        discard_fragment();
    }
    return texel * half4(in.tint);
}
