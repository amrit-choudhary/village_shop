/**
 * Evolution UI scene.
 */

#pragma once

#include "client/src/scene/scene_ui.h"

namespace ME {

class SceneUIEvolution : public SceneUI {
   public:
    SceneUIEvolution();
    virtual ~SceneUIEvolution() override;

    virtual void Init() override;
    virtual void CreateResources() override;
    virtual void BuildUISprites() override;
    virtual void BuildTextRenderers() override;
};

}  // namespace ME
