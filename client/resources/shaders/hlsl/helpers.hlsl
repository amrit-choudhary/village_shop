// contains some common helper functions and macros for HLSL shaders.

// Forward declarations.
float2 GetAtlasUVPad(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps);

// Function to calculate UV coordinates for a texture atlas.
// Atlas index is the index of the texture in the atlas, starting from top left.
// uv will simply be quad UVs from (0,0) to (1,1).
float2 GetAtlasUV(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps)
{
    if(atlasProps.padding > 0){
        return GetAtlasUVPad(uv, atlasIndex, atlasProps);
    }
    // Calculate tile size in UV space
    float tileWidthUV  = (float)atlasProps.tileSizeX / (float)atlasProps.width;
    float tileHeightUV = (float)atlasProps.tileSizeY / (float)atlasProps.height;

    // Find tile position (row, col) from atlasIndex
    uint col = atlasIndex % atlasProps.numTilesX;
    uint row = atlasIndex / atlasProps.numTilesX;

    // Offset in UV space
    float2 tileOffsetUV = float2(col * tileWidthUV, row * tileHeightUV);

    // Adjust input UV to fit within the tile
    float2 adjustedUV = tileOffsetUV + uv * float2(tileWidthUV, tileHeightUV);

    return adjustedUV;
}

float2 GetAtlasUVPad(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps)
{
    // Calculate tile size in UV space, tile size including padding
    float tileWidthUV  = (float)atlasProps.tileSizeX / (float)atlasProps.width;
    float tileHeightUV = (float)atlasProps.tileSizeY / (float)atlasProps.height;

    // Calculate padding in UV space
    float padU = (float)atlasProps.padding / (float)atlasProps.width;
    float padV = (float)atlasProps.padding / (float)atlasProps.height;

    // Find tile position (row, col) from atlasIndex
    uint col = atlasIndex % atlasProps.numTilesX;
    uint row = atlasIndex / atlasProps.numTilesX;

    // For first column/row, no padding at the edge
    float offsetU = col * tileWidthUV + (col > 0 ? padU : 0.0f);
    float offsetV = row * tileHeightUV + (row > 0 ? padV : 0.0f);

    // Usable area inside the tile (excluding padding on both sides except for first tile)
    float usableWidthUV  = tileWidthUV  - ((col > 0 ? 2.0f : 1.0f) * padU);
    float usableHeightUV = tileHeightUV - ((row > 0 ? 2.0f : 1.0f) * padV);

    float2 tileOffsetUV = float2(offsetU, offsetV);
    float2 adjustedUV = tileOffsetUV + uv * float2(usableWidthUV, usableHeightUV);

    return adjustedUV;
}
