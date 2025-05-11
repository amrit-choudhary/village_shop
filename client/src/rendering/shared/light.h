/**
 * Light class definition.
 * This class represents a light source in a 3D scene.
 * It contains properties such as position, color, intensity, and type of light.
 */

#pragma once

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

    void SetPosition(const ME::Vec3& position);
    ME::Vec3 GetPosition() const;

    void SetDirection(const ME::Vec3& direction);
    ME::Vec3 GetDirection() const;

    void SetColor(const ME::Vec3& color);
    ME::Vec3 GetColor() const;

    void SetIntensity(float intensity);
    float GetIntensity() const;

    LightType GetType() const;
    void SetType(LightType type);

   private:
    ME::Vec3 position;   // Position of the light in 3D space
    ME::Vec3 direction;  // Direction of the light (for directional and spotlight)
    ME::Vec3 color;      // Color of the light
    float intensity;     // Intensity of the light
    LightType type;      // Type of the light (ambient, directional, point, spotlight)
};

}  // namespace ME