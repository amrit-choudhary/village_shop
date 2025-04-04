#include "vec4.h"

#include "cmath"

using namespace ME::Math;

const Vec4 Vec4::Zero{0.0f, 0.0f, 0.0f, 0.0f};
const Vec4 Vec4::One{1.0f, 1.0f, 1.0f, 1.0f};

float ME::Math::Vec4::Length() {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

void ME::Math::Vec4::Normalise() {
    float len = Length();
    x = x / len;
    y = y / len;
    z = z / len;
    w = w / len;
}

void ME::Math::Vec4::NormaliseSafe() {
    float len = Length();
    if (len == 0) {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    } else {
        x = x / len;
        y = y / len;
        z = z / len;
        w = w / len;
    }
}

Vec4 ME::Math::Vec4::Normalised() {
    float len = Length();
    return (len > 0) ? Vec4{x / len, y / len, z / len, w / len} : Vec4{0, 0, 0, 0};
}

Vec4 ME::Math::Vec4::operator-() const {
    return Vec4{-x, -y, -z, -w};
}

Vec4& ME::Math::Vec4::operator+=(const Vec4& b) {
    x += b.x;
    y += b.y;
    z += b.z;
    w += b.w;
    return *this;
}

Vec4& ME::Math::Vec4::operator-=(const Vec4& b) {
    x -= b.x;
    y -= b.y;
    z -= b.z;
    w -= b.w;
    return *this;
}

Vec4& ME::Math::Vec4::operator*=(const Vec4& b) {
    x *= b.x;
    y *= b.y;
    z *= b.z;
    w *= b.w;
    return *this;
}

Vec4& ME::Math::Vec4::operator/=(const Vec4& b) {
    x /= b.x;
    y /= b.y;
    z /= b.z;
    w /= b.w;
    return *this;
}

float ME::Math::Vec4::Dot(const Vec4& a, const Vec4& b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

Vec4 ME::Math::operator+(const Vec4& a, const Vec4& b) {
    return Vec4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

Vec4 ME::Math::operator-(const Vec4& a, const Vec4& b) {
    return Vec4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Vec4 ME::Math::operator*(const Vec4& a, const Vec4& b) {
    return Vec4{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

Vec4 ME::Math::operator/(const Vec4& a, const Vec4& b) {
    return Vec4{a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

bool ME::Math::operator==(const Vec4& a, const Vec4& b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

bool ME::Math::operator!=(const Vec4& a, const Vec4& b) {
    return !(a == b);
}
