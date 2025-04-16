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
};

struct Uniforms {
    float4x4 transformationMatrix;
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
                            constant Uniforms& uniforms [[buffer(1)]]) {

    float3x3 normalMatrix = transpose(inverse3x3(float3x3(
                                                uniforms.transformationMatrix[0].xyz,
                                                uniforms.transformationMatrix[1].xyz,
                                                uniforms.transformationMatrix[2].xyz)));
    
    VertexOut out;
    out.position = uniforms.transformationMatrix * float4(in.position, 1.0);
    out.normal = float4(normalMatrix * in.normal, 1.0);
    // out.color = float3(abs(out.normal.x), abs(out.normal.y), abs(out.normal.z));
    float light = 0.07 + out.normal.x;
    out.color = float3(light, light, light);
    return out;
}

fragment float4 fragmentMain(VertexOut in [[stage_in]]) {
    return float4(in.color.x, in.color.y, in.color.z, 1.0);
}
