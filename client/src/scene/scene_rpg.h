#pragma once

/**
 * RPG Game Scene.
 */

#include "scene.h"
#include "src/math/vec2.h"

namespace ME {

class SceneRPG : public Scene {
   public:
    SceneRPG();
    virtual ~SceneRPG() override;

   private:
    const size_t gridWidth = 48;
    const size_t gridHeight = 100;
    const size_t gridCount = gridWidth * gridHeight;
    const size_t cellPadding = 0;
    const size_t cellSize = 16;
    const size_t cellSizeby2 = cellSize / 2;
    const int32_t originX = -400;
    const int32_t originY = -100;

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
};

}  // namespace ME
