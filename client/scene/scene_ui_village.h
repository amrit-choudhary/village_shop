/**
 * The village HUD: a title bar for now. Uses the engine's UI atlas and font at the texture slots the DX12
 * UI passes expect (sprites at 0, text at 1).
 */
#pragma once

#include "moksha/client/scene/scene_ui.h"

namespace ME {
class Label;
class Panel;
}  // namespace ME

namespace VS {

class SceneUIVillage : public ME::SceneUI {
   public:
    SceneUIVillage() = default;
    ~SceneUIVillage() override;

    SceneUIVillage(const SceneUIVillage&) = delete;
    SceneUIVillage& operator=(const SceneUIVillage&) = delete;
    SceneUIVillage(SceneUIVillage&&) = delete;
    SceneUIVillage& operator=(SceneUIVillage&&) = delete;

    void CreateResources() override;
    void BuildUIElements() override;

   private:
    ME::Panel* titleBar = nullptr;
    ME::Label* titleLabel = nullptr;
};

}  // namespace VS
