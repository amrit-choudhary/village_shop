#include "light.h"

ME::Light::Light() {
    position = ME::Vec3{0.0f, 0.0f, 0.0f};
    direction = ME::Vec3{0.0f, 0.0f, 1.0f};
    color = ME::Vec3{1.0f, 1.0f, 1.0f};
    intensity = 1.0f;
    type = LightType::Directional;
}

ME::Light::~Light() {}

void ME::Light::SetPosition(const ME::Vec3& position) {
    this->position = position;
}

ME::Vec3 ME::Light::GetPosition() const {
    return position;
}

void ME::Light::SetDirection(const ME::Vec3& direction) {
    this->direction = direction;
}

ME::Vec3 ME::Light::GetDirection() const {
    return direction;
}

void ME::Light::SetColor(const ME::Vec3& color) {
    this->color = color;
}
ME::Vec3 ME::Light::GetColor() const {
    return color;
}
void ME::Light::SetIntensity(float intensity) {
    this->intensity = intensity;
}
float ME::Light::GetIntensity() const {
    return intensity;
}
ME::LightType ME::Light::GetType() const {
    return type;
}
void ME::Light::SetType(LightType type) {
    this->type = type;
}
