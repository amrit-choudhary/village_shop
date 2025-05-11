/**
 * Light class definition.
 * This class represents a light source in a 3D scene.
 * It contains properties such as position, color, intensity, and type of light.
 */

#pragma once

#include "color.h"
#include "src/math/vec3.h"

namespace ME {

enum class LightType {
    Ambient,
    Directional,
    Point,
    Spotlight,
};

class Light {
   public:
    Light();
    ~Light();

    ME::Vec3 position;
    ME::Vec3 direction;
    ME::Color color;
    float intensity;
    LightType type;
};

}  // namespace ME
