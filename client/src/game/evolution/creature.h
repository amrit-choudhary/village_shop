/**
 * Creature class.
 */

#pragma once

#include <shared/src/datastructure/grid.h>
#include <shared/src/math/vec2.h>
#include <shared/src/random/random_engine.h>

namespace ME {

class Creature {
   public:
    Creature();
    virtual ~Creature();

    Vec2 position = Vec2::Zero;

    // Ticks the movement cooldown; once it elapses, tries to move to a random
    // walkable neighbor tile and resets the cooldown.
    void Update(float deltaTime, const Grid<uint8_t>* walkableMap, Random& rnd);

   protected:
    float health = 100.0f;
    float speed = 5.0f;
    float moveCooldown = 1.0f;

    static constexpr float kMoveCooldownDuration = 1.0f;

    void Move(const Grid<uint8_t>* walkableMap, Random& rnd);
};

}  // namespace ME
