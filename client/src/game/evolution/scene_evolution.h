/**
 * Evolution scene.
 */

#pragma once

#include <shared/src/datastructure/grid.h>
#include <shared/src/math/math.h>

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
    const float originX = -50.0f;
    const float originY = -50.0f;

    // Game params.
    const size_t oceanMapSize = ME::POW2(4);
    const size_t biomeMapSize = ME::POW2(8);
    const size_t mapSize = ME::POW2(12);

    ME::Grid<uint8_t>* terrain;
    ME::Grid<uint8_t>* oceanBase;
    ME::Grid<uint8_t>* ocean;
    ME::Grid<uint8_t>* biome;
};

}  // namespace ME
