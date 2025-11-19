#pragma once

/**
 * Game of Life scene.
 */

#include "scene.h"
#include "src/math/vec2.h"

namespace ME {

class SceneGameOfLife : public Scene {
   public:
    SceneGameOfLife();
    virtual ~SceneGameOfLife() override;

   private:
    const size_t gridWidth = 100;
    const size_t gridHeight = 100;
    const size_t gridCount = gridWidth * gridHeight;
    const size_t cellPadding = 1;
    const size_t cellSize = 17;
    const size_t cellSizeby2 = cellSize / 2;
    const int32_t originX = -900;
    const int32_t originY = -900;

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
};

}  // namespace ME
