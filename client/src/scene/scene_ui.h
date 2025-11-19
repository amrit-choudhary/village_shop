#pragma once

/**
 * UI scene class implementation.
 * This will have ui related things like sprites and texts.
 */

#include "../rendering/shared/camera.h"
#include "../rendering/shared/sprite_renderer.h"
#include "../rendering/shared/text_renderer.h"
#include "../rendering/shared/texture.h"
#include "src/physics/collider_aabb.h"

namespace ME {

class SceneUI {
   public:
    SceneUI();
    virtual ~SceneUI();

    /**
     * Updates the rendering related data in the UI scene, if any render element is marked as dirty.
     * This is called from Renderer.Update(), right before Renderer.Draw().
     * Like if sprite renderer's atlas index changed due to animation.
     * This is called every frame before rendering.
     * Everything that is not DX or MTL dependent is updated here, after which DX and MTL scene is updated from
     * Renderer.Update().
     */
    void Update();

   public:
    // Resources
    const char** spriteTexturePaths;
    ME::TextureAtlasProperties* textureAtlasProperties;

    ME::Transform** uiSpriteTransforms;
    ME::SpriteRenderer** uiSpriteRenderers;
    ME::UISpriteRendererInstanceData** uiSpriteInstanceData;

    ME::Transform** textTransforms;
    ME::TextRenderer** textRenderers;
    ME::TextRendererInstanceData** textInstanceData;

    uint8_t textureAtlasPropertiesCount = 0;
    uint8_t spriteTextureCount = 0;

    uint32_t uiSpriteRendererCount = 0;
    uint32_t uiSpriteInstanceDataCount = 0;
    uint32_t textRendererCount = 0;
    uint32_t textTransformsCount = 0;
    uint32_t textInstanceDataCount = 0;

    virtual void Init();
    virtual void CreateResources();
    virtual void BuildUISprites();
    virtual void BuildTextRenderers();

   private:
    /**
     * Updates UI sprite renderers that are marked as dirty.
     * After update, the dirty flag is cleared.
     */
    void UpdateUISpriteRenderers();

    /**
     * Updates text renderers that are marked as dirty.
     * After update, the dirty flag is cleared.
     */
    void UpdateTextRenderers();
};

}  // namespace ME
