#pragma once

/**
 * SceneUI for GameUIDemo. Loads the UI atlas + font textures at the fixed indices the DX12
 * renderer hardcodes for UI drawing (spriteTextures[0]/textureAtlasProperties[0] for sprites,
 * [1]/[1] for text — see renderer_dx.cpp's UI Sprite/Text Drawing sections). No BuildUISprites/
 * BuildTextRenderers override — content comes from the dynamic UIElement tree, built in
 * BuildUIElements() below rather than in GameUIDemo, matching how SceneUIHUD builds its own
 * static content in BuildUISprites/BuildTextRenderers rather than in a Game subclass.
 */

#include "scene_ui.h"
#include "src/ui/button.h"
#include "src/ui/image.h"
#include "src/ui/label.h"
#include "src/ui/panel.h"

namespace ME {

class SceneUIDemo : public ME::SceneUI {
   public:
    SceneUIDemo();
    virtual ~SceneUIDemo() override;

    virtual void CreateResources() override;
    virtual void BuildUIElements() override;

    // GameUIDemo needs these to update from input each frame; topBarPanel/titleLabel are never
    // touched after creation.
    ME::Label* GetHealthLabel() const;
    ME::Label* GetScoreLabel() const;
    ME::Image* GetCenterImage() const;
    ME::Button* GetScoreButton() const;

   private:
    ME::Panel* topBarPanel = nullptr;
    ME::Label* titleLabel = nullptr;
    ME::Image* centerImage = nullptr;
    ME::Label* healthLabel = nullptr;
    ME::Label* scoreLabel = nullptr;

    ME::Button* scoreButton = nullptr;
    ME::Panel* scoreButtonPanel = nullptr;
    ME::Label* scoreButtonLabel = nullptr;
};

}  // namespace ME
