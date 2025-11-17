#pragma once

/**
 * Character test scene.
 */

#include "scene.h"

namespace ME {

class SceneCharacterTest : public Scene {
   public:
    SceneCharacterTest();
    virtual ~SceneCharacterTest() override;

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
    const float charWidth = 8.0f;
    const float charHeight = 12.0f;
    const float npcWidth = 6.0f;
    const float npcHeight = 6.0f;
    const size_t maxNPCCount = 124;
};

};  // namespace ME
