#include "light.h"

ME::Light::Light() {
    position = ME::Vec3{0.0f, 0.0f, 0.0f};
    direction = ME::Vec3{0.0f, 0.0f, 1.0f};
    color = ME::Color(1.0f, 1.0f, 1.0f);
    intensity = 1.0f;
    type = LightType::Directional;
}

ME::Light::~Light() {}
