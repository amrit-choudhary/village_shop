#include "camera.h"

#include <cmath>

ME::Camera::Camera() {
    position = ME::Math::Vec3::Zero;
    rotation = ME::Math::Vec4{0.0f, 0.0f, 0.0f, 1.0f};
    up = ME::Math::Vec3::Up;
    viewPosition = ME::Math::Vec3{0.0f, 0.0f, 1000.0f};

    fov = 90.0f;
    aspectRatio = 1.3333f;
    nearPlane = 0.1f;
    farPlane = 10'000.0f;
}

ME::Camera::~Camera() {}

ME::Math::Mat4 ME::Camera::GetViewMatrix() {
    return ME::Math::Mat4::View(ME::Math::Vec4{position.x, position.y, position.z, 1.0f},
                                ME::Math::Vec4{viewPosition.x, viewPosition.y, viewPosition.z, 1.0f},
                                ME::Math::Vec4{up.x, up.y, up.z, 1.0f});
}

ME::Math::Mat4 ME::Camera::GetProjectionMatrix() {
    return ME::Math::Mat4::Perspective(fov * (M_PI / 180.0f), aspectRatio, nearPlane, farPlane);
}
