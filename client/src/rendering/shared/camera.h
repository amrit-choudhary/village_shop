/**
 * Camera class
 */

#pragma once

#include "src/math/matrix4.h"
#include "src/math/vec3.h"
#include "src/math/vec4.h"

namespace ME {

enum class ProjectionType {
    Perspective,
    Orthographic,
};

/**
 * Camera class for rendering.
 */
class Camera {
   public:
    Camera();
    ~Camera();

    ProjectionType projectionType;

    ME::Mat4 GetViewMatrix();
    ME::Mat4 GetProjectionMatrix();

    ME::Vec3 position;
    ME::Vec4 rotation;
    ME::Vec3 up;
    ME::Vec3 viewPosition;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    float orthographicSize;

   private:
};

}  // namespace ME
