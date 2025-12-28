#pragma once

/**
 * Geometry definitions for UI elements.
 */

#include "src/math/vec2.h"

namespace ME {

class Geometry {
   public:
    Geometry();
    ~Geometry();

    // Getters and setters for position and size
    Vec2 GetPosition() const;
    void SetPosition(const Vec2& pos);
    Vec2 GetAbsolutePosition() const;
    void SetAbsolutePosition(const Vec2& absPos);
    Vec2 GetSize() const;
    void SetSize(const Vec2& sz);
    Vec2 GetAbsoluteSize() const;
    void SetAbsoluteSize(const Vec2& absSz);

   private:
    Vec2 position = Vec2();          // local position
    Vec2 size = Vec2();              // local size
    Vec2 absolutePosition = Vec2();  // absolute position in the UI
    Vec2 absoluteSize = Vec2();      // absolute size in the UI
};

}  // namespace ME
