#pragma once

/**
 * Physics scene for the Character Test game.
 */

#include "../scene/scene_character_test.h"
#include "src/physics/collider_aabb.h"
#include "src/physics/physics_scene.h"

namespace ME {

class PhysicsSceneChar : public ME::PhysicsScene {
   public:
    PhysicsSceneChar();
    virtual ~PhysicsSceneChar() override;

    // Initializes the physics scene with collider objects specific to Character Test.
    virtual void Init() override;

    // Initializes the physics scene with a given scene specific to Character Test.
    virtual void Init(const ME::Scene* scene) override;
};

}  // namespace ME
