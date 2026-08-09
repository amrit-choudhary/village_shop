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
};

}  // namespace ME
