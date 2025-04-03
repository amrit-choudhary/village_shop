#include "vec3.h"

#include "cmath"

using namespace ME::Math;

const Vec3 Vec3::Zero{0.0f, 0.0f, 0.0f};
const Vec3 Vec3::One{1.0f, 1.0f, 1.0f};

const Vec3 Vec3::Up{0.0f, 1.0f, 0.0f};
const Vec3 Vec3::Down{0.0f, -1.0f, 0.0f};

const Vec3 Vec3::Right{1.0f, 0.0f, 0.0f};
const Vec3 Vec3::Left{-1.0f, 0.0f, 0.0f};

const Vec3 Vec3::Forward{0.0f, 0.0f, 1.0f};
const Vec3 Vec3::Backward{0.0f, 0.0f, -1.0f};

float ME::Math::Vec3::Length() {
    return std::sqrt(x * x + y * y + z * z);
}

void ME::Math::Vec3::Normalise() {
    float len = Length();
    x = x / len;
    y = y / len;
    z = z / len;
}

Vec3 ME::Math::Vec3::Normalised() {
    float len = Length();
    x = x / len;
    y = y / len;
    z = z / len;

    return Vec3{x, y, z};
}

Vec3 ME::Math::Vec3::operator-() const {
    return Vec3{-x, -y, -z};
}

Vec3& ME::Math::Vec3::operator+=(const Vec3& b) {
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

Vec3& ME::Math::Vec3::operator-=(const Vec3& b) {
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return *this;
}

Vec3& ME::Math::Vec3::operator*=(const Vec3& b) {
    x *= b.x;
    y *= b.y;
    z *= b.z;
    return *this;
}

Vec3& ME::Math::Vec3::operator/=(const Vec3& b) {
    x /= b.x;
    y /= b.y;
    z /= b.z;
    return *this;
}

Vec3 ME::Math::operator+(const Vec3& a, const Vec3& b) {
    return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 ME::Math::operator-(const Vec3& a, const Vec3& b) {
    return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 ME::Math::operator*(const Vec3& a, const Vec3& b) {
    return Vec3{a.x * b.x, a.y * b.y, a.z * b.z};
}

Vec3 ME::Math::operator/(const Vec3& a, const Vec3& b) {
    return Vec3{a.x / b.x, a.y / b.y, a.z / b.z};
}

bool ME::Math::operator==(const Vec3& a, const Vec3& b) {
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return true;
    } else {
        return false;
    }
}

bool ME::Math::operator!=(const Vec3& a, const Vec3& b) {
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return false;
    } else {
        return true;
    }
}
