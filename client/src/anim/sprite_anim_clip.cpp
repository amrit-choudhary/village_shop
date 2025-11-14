#include "sprite_anim_clip.h"

ME::SpriteAnimClip::SpriteAnimClip(bool bLooping, uint8_t textureIndex, const uint8_t spriteCount,
                                   const uint16_t* spriteIndices)
    : bLooping(bLooping), textureIndex(textureIndex), spriteCount(spriteCount), spriteIndices(spriteIndices) {
    // vars are initialized in initializer list because some are constant.
}

ME::SpriteAnimClip::~SpriteAnimClip() {
    delete[] spriteIndices;
    spriteIndices = nullptr;
}

uint8_t ME::SpriteAnimClip::GetSpriteCount() const {
    return spriteCount;
}

bool ME::SpriteAnimClip::IsLooping() const {
    return bLooping;
}

uint16_t ME::SpriteAnimClip::GetSpriteAtlasIndex(uint8_t index) const {
    if (spriteCount == 0) {
        return 0;
    }
    if (index < spriteCount) {
        return spriteIndices[index];
    }
    return spriteIndices[0];
}
