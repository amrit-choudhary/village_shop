/**
 * Transform class which represents a 3D transformation in space.
 * It includes translation, rotation, and scaling operations.
 */

#pragma once

#include "src/math/matrix4.h"
#include "src/math/vec3.h"
#include "src/math/vec4.h"

namespace ME {

/**
 * Transform class for handling 3D transformations.
 * Contains position, rotation, and scale.
 * Use this to manipulate the position, rotation, and scale of objects in 3D space.
 * Will return the transformation matrix / model matrix.
 */
class Transform {
   public:
    Transform();
    ~Transform();

    void SetPosition(float x, float y, float z);

    void SetRotation(float x, float y, float z, float w = 1.0f);

    void SetScale(float x, float y, float z);

    ME::Math::Mat4 GetModelMatrix() const {
        // Create the model matrix using position, rotation, and scale.
        ME::Math::Mat4 translationMat =
            ME::Math::Mat4::Translation(ME::Math::Vec4{position.x, position.y, position.z, 1.0f});
        ME::Math::Mat4 rotationMat = ME::Math::Mat4::Rotation(rotation);
        ME::Math::Mat4 scaleMat = ME::Math::Mat4::Scale(ME::Math::Vec4{scale.x, scale.y, scale.z, 1.0f});
        return translationMat * rotationMat * scaleMat;
    }

   private:
    ME::Math::Vec3 position;
    ME::Math::Vec4 rotation;
    ME::Math::Vec3 scale;
};

}  // namespace ME
