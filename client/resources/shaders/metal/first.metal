#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float2 uv    [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 normal;
    float3 color;
};

vertex VertexOut vertexMain(VertexIn in [[stage_in]]) {
    VertexOut out;
    out.position = float4(in.position * 5.0, 1.0);
    out.normal = in.normal;
    //out.color = float3(abs(in.normal.x), abs(in.normal.y), abs(in.normal.z));
    float light = in.normal.y * 0.5 + 0.5;
    out.color = float3(light, light, light);
    return out;
}

fragment float4 fragmentMain(VertexOut in [[stage_in]]) {
    return float4(in.color.x, in.color.y, in.color.z, 1.0);
}
