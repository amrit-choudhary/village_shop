#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float2 uv    [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float4 normal;
    float3 color;
    float2 uv;
};

// Compute the determinant of a 3x3 matrix
float determinant3x3(float3x3 m) {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

// Compute the inverse of a 3x3 matrix
float3x3 inverse3x3(float3x3 m) {
    float det = determinant3x3(m);
    float3x3 result;
    
    // Check for non-invertible matrix
    if (abs(det) < 1e-6) {
        return float3x3(1.0); // Return identity matrix
    }
    
    float invDet = 1.0 / det;
    
    result[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    result[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    result[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
    result[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    result[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    result[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
    result[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    result[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
    result[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;
    
    return result;
}

vertex VertexOut vertexMain(VertexIn in [[stage_in]], 
                            constant float4x4& modelMatrix      [[buffer(1)]],
                            constant float4x4& viewMatrix       [[buffer(2)]],
                            constant float4x4& projectionMatrix [[buffer(3)]],
                            constant float4* instances          [[buffer(4)]],
                            uint instanceID                     [[instance_id]]) {

    float3x3 normalMatrix = transpose(inverse3x3(float3x3(
                                                modelMatrix[0].xyz,
                                                modelMatrix[1].xyz,
                                                modelMatrix[2].xyz)));

    VertexOut out;
    float4 posM = modelMatrix * float4(in.position, 1.0f);
    float4 posI = posM + float4(instances[instanceID].xyz * 3, 0.0f);
    out.position = projectionMatrix * viewMatrix * posI;
    out.normal = float4(normalMatrix * in.normal, 1.0);
    out.normal.xyz = normalize(out.normal.xyz);
    out.uv = in.uv;
    return out;
}

fragment half4 fragmentMain(VertexOut in [[stage_in]],
                            texture2d<half, access::sample> tex [[texture(0)]]) {

    constexpr sampler s(address::repeat, filter::linear);
    half3 texel =  tex.sample(s, in.uv).rgb;
    
    
    // assume light coming from (front-top-right)
    float3 l = normalize(float3( 1.0, 1.0, -0.8 ));
    float3 n = normalize( in.normal.xyz );

    float ndotl = saturate( dot( n, l ) ) + 0.05f;
    return half4( ndotl * texel.r, ndotl * texel.g, ndotl * texel.b, 1.0 );
}
