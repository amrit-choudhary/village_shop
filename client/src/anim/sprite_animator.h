#pragma once

#include <cstdint>

#include "sprite_anim_clip.h"

namespace ME {

class SpriteAnimator {
   public:
    SpriteAnimator(uint8_t frameRate = 24);
    virtual ~SpriteAnimator();

    /**
     * Add an animation clip to the animator.
     * NOTE:
     * Create SpriteAnimClip using 'new' and pass the pointer to this method.
     * Don't store the pointer elsewhere, as the animator will take ownership and delete it.
     * Dont send stack allocated or global/static clips as they can get deleted automatically.
     */
    void AddClip(ME::SpriteAnimClip* clip);

    /**
     * Change the active animation clip by index.
     */
    void ChangeClip(uint8_t clipIndex);

    // Game frame update.
    void Update(double deltaTime);

    /**
     * Set the frame rate (frames per second) for the animator.
     * Setting it to 0 is not possible, will set it to defaultFrameRate.
     */
    void SetFrameRate(uint8_t frameRate);

    /**
     * Return the resultant current frame atlasIndex of the active animation clip.
     */
    uint16_t GetCurrentAtlasIndex() const;

    /**
     * Return the current animation clip index.
     */
    uint8_t GetCurrentClipIndex() const;

   private:
    uint8_t currentFrame = 0;
    uint8_t clipCount = 0;
    uint8_t currentClipIndex = 0;
    ME::SpriteAnimClip** clips = nullptr;
    uint8_t accumulatedFrames = 0;
    uint8_t skipFrameCount = 0;
    uint8_t frameRate = DEFAULT_FRAME_RATE;

    constexpr static uint8_t DEFAULT_FRAME_RATE = 24;
};

}  // namespace ME
