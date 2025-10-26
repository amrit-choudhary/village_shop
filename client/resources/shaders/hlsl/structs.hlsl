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

// Per Instance Data for Sprite Renderer
// uint = uint32_t in c++ because uint16_t is not well supported in HLSL.
struct SpriteInstanceData {
    float4x4 modelMatrix;
    float4 color;
    uint atlasIndex;
    uint3 padding; 
};

// Texture Atlas Properties
struct TextureAtlasProperties {
    uint tileSizeX;
    uint tileSizeY;
    uint padding;
    uint numTextures;
    uint numTilesX;
    uint numTilesY;
    uint width;
    uint height;
};
