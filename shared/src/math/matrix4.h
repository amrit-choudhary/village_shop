/**
 * 4 x 4 Matrix
 */

#pragma once

namespace ME::Math {

class Vec4;

/**
 * 4 x 4 Matrix class.
 */

class Mat4 {
   public:
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    Vec4 GetRow(const int row) const;

    Vec4 GetColumn(const int column) const;
};

}  // namespace ME::Math
