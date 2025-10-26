// contains some common helper functions and macros for HLSL shaders.

// Function to calculate UV coordinates for a texture atlas.
// Atlas index is the index of the texture in the atlas, starting from top left.
// uv will simply be quad UVs from (0,0) to (1,1).
float2 GetAtlasUV(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps)
{
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
