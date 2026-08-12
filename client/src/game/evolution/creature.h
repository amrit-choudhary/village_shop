/**
 * Creature class.
 */

#pragma once

#include <shared/src/math/vec2.h>

namespace ME {

class Creature {
   public:
    Creature();
    virtual ~Creature();

    Vec2 position = Vec2::Zero;

    void Update(float deltaTime);

   protected:
    float health = 100.0f;
    float speed = 5.0f;

    void Move();
};

}  // namespace ME
