/**
 * Camera class
 */

#pragma once

#include "src/math/matrix4.h"
#include "src/math/vec3.h"
#include "src/math/vec4.h"

namespace ME {

/**
 * Camera class for rendering.
 */
class Camera {
   public:
    Camera();
    ~Camera();

    ME::Math::Mat4 GetViewMatrix();
    ME::Math::Mat4 GetProjectionMatrix();

    ME::Math::Vec3 position;
    ME::Math::Vec4 rotation;
    ME::Math::Vec3 up;
    ME::Math::Vec3 viewPosition;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

   private:
};

}  // namespace ME
