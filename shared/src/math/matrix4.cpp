#include "matrix4.h"

#include "cmath"
#include "vec4.h"

using namespace ME::Math;

Vec4 Mat4::GetRow(const int row) const {
    switch (row) {
        case 0:
            return Vec4{m00, m01, m02, m03};
        case 1:
            return Vec4{m10, m11, m12, m13};
        case 2:
            return Vec4{m20, m21, m22, m23};
        case 3:
            return Vec4{m30, m31, m32, m33};
        default:
            return Vec4{0.0f, 0.0f, 0.0f, 0.0f};
    }
}
Vec4 Mat4::GetColumn(const int column) const {
    switch (column) {
        case 0:
            return Vec4{m00, m10, m20, m30};
        case 1:
            return Vec4{m01, m11, m21, m31};
        case 2:
            return Vec4{m02, m12, m22, m32};
        case 3:
            return Vec4{m03, m13, m23, m33};
        default:
            return Vec4{0.0f, 0.0f, 0.0f, 0.0f};
    }
}