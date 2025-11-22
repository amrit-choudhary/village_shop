#pragma once

/**
 * UI HUD scene class implementation.
 */

#include "scene_ui.h"

namespace ME {

class SceneUIHUD : public ME::SceneUI {
   public:
    SceneUIHUD();
    virtual ~SceneUIHUD() override;

    virtual void Init() override;
    virtual void CreateResources() override;
    virtual void BuildUISprites() override;
    virtual void BuildTextRenderers() override;
};

}  // namespace ME
