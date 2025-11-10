// contains some common helper functions and macros for HLSL shaders.

// Forward declarations.
float2 GetAtlasUVPad(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps, uint flags);

// Function to calculate UV coordinates for a texture atlas.
// Atlas index is the index of the texture in the atlas, starting from top left.
// uv will simply be quad UVs from (0,0) to (1,1).

float2 GetAtlasUV(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps, uint flags)
{
    if (atlasProps.padding > 0) {
        return GetAtlasUVPad(uv, atlasIndex, atlasProps, flags);
    }

    // Calculate tile size in UV space
    float tileWidthUV  = (float)atlasProps.tileSizeX / (float)atlasProps.width;
    float tileHeightUV = (float)atlasProps.tileSizeY / (float)atlasProps.height;

    // Find tile position (row, col) from atlasIndex
    uint col = atlasIndex % atlasProps.numTilesX;
    uint row = atlasIndex / atlasProps.numTilesX;

    // Offset in UV space (start of tile)
    float2 tileOffsetUV = float2(col * tileWidthUV, row * tileHeightUV);

    // Apply flipping by inverting local uv; do NOT change the tile offset.
    if ((flags & 0x1) != 0) uv.x = 1.0f - uv.x; // horizontal flip
    if ((flags & 0x2) != 0) uv.y = 1.0f - uv.y; // vertical flip

    // Adjust input UV to fit within the tile
    float2 adjustedUV = tileOffsetUV + uv * float2(tileWidthUV, tileHeightUV);

    return adjustedUV;
}

float2 GetAtlasUVPad(float2 uv, uint atlasIndex, TextureAtlasProperties atlasProps, uint flags)
{
    // tile content size in UV
    float tileWidthUV  = (float)atlasProps.tileSizeX / (float)atlasProps.width;
    float tileHeightUV = (float)atlasProps.tileSizeY / (float)atlasProps.height;

    // padding in UV and stride between tiles (content + padding)
    float padU = (float)atlasProps.padding / (float)atlasProps.width;
    float padV = (float)atlasProps.padding / (float)atlasProps.height;
    float strideU = tileWidthUV + padU;
    float strideV = tileHeightUV + padV;

    // tile coordinates
    uint col = atlasIndex % atlasProps.numTilesX;
    uint row = atlasIndex / atlasProps.numTilesX;

    // offset is col/row * stride; no extra special-casing for edges required
    float offsetU = col * strideU;
    float offsetV = row * strideV;

    // flipping only affects the local UV inside the tile; no offset adjustment needed
    if ((flags & 0x1) != 0) uv.x = 1.0f - uv.x;
    if ((flags & 0x2) != 0) uv.y = 1.0f - uv.y;

    // map local uv into atlas using tile content size (padding sits between tiles)
    float2 adjustedUV = float2(offsetU + uv.x * tileWidthUV, offsetV + uv.y * tileHeightUV);
    return adjustedUV;
}
