#pragma once

/**
 * SceneUI for GameNetTest. Same fixed atlas/font texture indices as SceneUIDemo (see
 * scene_ui_demo.h) - content comes from the dynamic UIElement tree built in
 * BuildUIElements() below.
 */

#include "scene_ui.h"
#include "client/src/ui/button.h"
#include "client/src/ui/label.h"
#include "client/src/ui/panel.h"

namespace ME {

class SceneUINetTest : public ME::SceneUI {
   public:
    SceneUINetTest();
    virtual ~SceneUINetTest() override;

    virtual void CreateResources() override;
    virtual void BuildUIElements() override;

    ME::Label* GetStatusLabel() const;
    ME::Label* GetPingLabel() const;
    ME::Label* GetYourScoreLabel() const;
    ME::Label* GetOpponentScoreLabel() const;
    ME::Button* GetScoreButton() const;

   private:
    ME::Panel* topBarPanel = nullptr;
    ME::Label* titleLabel = nullptr;
    ME::Label* statusLabel = nullptr;
    ME::Label* pingLabel = nullptr;
    ME::Label* yourScoreLabel = nullptr;
    ME::Label* opponentScoreLabel = nullptr;

    ME::Button* scoreButton = nullptr;
    ME::Panel* scoreButtonPanel = nullptr;
    ME::Label* scoreButtonLabel = nullptr;
};

}  // namespace ME
