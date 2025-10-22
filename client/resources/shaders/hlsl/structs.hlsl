// This contains hlsl version of game side structs used for rendering.
// Include this file in your hlsl shaders to get access to these structs.

// Ambient light data.
struct LightDataAmbient {
    float4 color;
    float intensity;
    float3 padding;
};

// Directional light data.
struct LightDataDirectional {
    float3 direction;
    float padding1;
    float intensity;
    float3 padding2;
    float4 color;
};
