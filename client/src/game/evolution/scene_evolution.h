/**
 * Evolution scene.
 */

#pragma once

#include <shared/src/datastructure/grid.h>
#include <shared/src/datastructure/pool.h>
#include <shared/src/math/math.h>
#include <shared/src/random/random_engine.h>

#include "client/src/scene/scene.h"
#include "creature.h"

namespace ME {

enum class TerrainType : uint8_t {
    Grass = 0,
    DarkGrass = 1,
    Forest = 2,
    Rocky = 3,
    Dirst = 4,
    Desert = 5,
    Beach = 6,
    Lake = 7,
    Ocean = 8
};

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

    // Ticks every creature's movement cooldown/AI and syncs moved creatures'
    // instanced sprite transforms. Called every frame from GameEvolution::Update.
    void UpdateCreatures(float deltaTime);

   private:
    const float tileSize = 12.0f;
    const float tileSizeHalf = tileSize / 2.0f;
    const float originX = -150.0f;
    const float originY = -150.0f;

    // Game params.
    const size_t oceanMapSize = ME::POW2(4);
    const size_t biomeMapSize = ME::POW2(5);
    const size_t mapSize = ME::POW2(8);
    const size_t creatureCount = 20'000;
    const size_t initialCreatureCount = 1'000;
    ME::Vec3 creatureParkPos{9000.0f, 9000.0f, 0.0f};

    ME::Grid<TerrainType>* terrain = nullptr;
    ME::Grid<TerrainType>* oceanBase = nullptr;
    ME::Grid<TerrainType>* ocean = nullptr;
    ME::Grid<TerrainType>* biome = nullptr;
    ME::Grid<uint8_t>* walkableMap = nullptr;

    ME::Pool<Creature>* creaturePool = nullptr;
};

}  // namespace ME
