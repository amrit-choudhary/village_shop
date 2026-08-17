/**
 * Creature class.
 */

#pragma once

#include <shared/src/datastructure/grid.h>
#include <shared/src/math/vec2.h>
#include <shared/src/random/random_engine.h>

namespace ME {

enum class CreatureState {
    None = 0,
    Idle = 1,
    Moving = 2,
    Attacking = 3,
    Reproducing = 4,
    Dying = 5,
    Dead = 6
};

class Creature {
   public:
    Creature();
    virtual ~Creature();
    // Copy constructor, used for pool management.
    Creature(const Creature& other);
    Creature& operator=(const Creature& other);

    Vec2 position = Vec2::Zero;
    CreatureState state = CreatureState::None;

    // Ticks the movement cooldown; once it elapses, tries to move to a random
    // walkable neighbor tile and resets the cooldown.
    void Update(float deltaTime, const Grid<uint8_t>* walkableMap, Random& rnd);

    void Spawn(const Vec2& spawnPosition, Random& rnd);

    float health = 100.0f;
    float speed = 5.0f;
    float moveCooldown = 1.0f;
    float lifetime = 0.0f;
    float maxLifetime = 10.0f;
    float reproCooldown = 0.0f;
    float maxReproCooldown = 10.0f;

    static constexpr float kMoveCooldownDuration = 1.0f;

    void Move(const Grid<uint8_t>* walkableMap, Random& rnd);
};

}  // namespace ME
