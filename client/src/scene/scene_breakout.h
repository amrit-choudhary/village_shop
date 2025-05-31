/**
 * Breakout game scene.
 */

#pragma once

#include "scene.h"

namespace ME {

class SceneBreakout : public Scene {
   public:
    SceneBreakout();
    virtual ~SceneBreakout() override;

   public:
    virtual void Init() override;
    virtual void CreateResources() override;
    virtual void BuildLights() override;
    virtual void BuildCamera() override;
    virtual void BuildTransforms() override;
    virtual void BuildMeshRenderers() override;
    virtual void BuildSpriteTransforms() override;
    virtual void BuildSpriteRenderers() override;
    virtual void BuildInstancedSpriteTransforms() override;
    virtual void BuildInstancedSpriteRenderers() override;
    virtual void BuildTextRenderers() override;

   private:
    void CreatePaddle();
    void CreateBall();
};

}  // namespace ME