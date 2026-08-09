/**
 * Evolution scene.
 */

#pragma once

#include "client/src/scene/scene.h"

namespace ME {

class SceneEvolution : public Scene {
   public:
    SceneEvolution();
    virtual ~SceneEvolution() override;

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

    virtual const char* GetDisplayName() const override;

   private:
    const float tileSize = 8.0f;
    const float tileSizeHalf = tileSize / 2.0f;
    const float originX = -200.0f;
    const float originY = -200.0f;
};

}  // namespace ME
