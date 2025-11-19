#pragma once

/**
 * Simple Dice Game Scene.
 */

#include "scene.h"

namespace ME {

class SceneDiceSimple : public Scene {
   public:
    SceneDiceSimple();
    virtual ~SceneDiceSimple() override;

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

   private:
    const float cellSize = 16.0f;
    const float diceGap = 20.0f;
};

}  // namespace ME
