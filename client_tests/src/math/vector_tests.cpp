/**
 * Contains test for Vector classes.
 */

#include <iostream>

#include "../test_runner.h"
#include "src/math/math.h"

TEST(Vector, Up) {
    ME::Math::Vec3 vec3 = ME::Math::Vec3::Up;
    EXPECT_EQ(vec3.x == 0.0f);
    EXPECT_EQ(vec3.y == 1.0f);
    EXPECT_EQ(vec3.z == 0.0f);
}

TEST(Vector, Norm) {
    ME::Math::Vec3 vec3{3.0f, 4.0f, 0.0f};
    vec3.Normalise();
    EXPECT_EQ((vec3 == ME::Math::Vec3{0.6f, 0.8f, 0.0f}));
}

TEST(Vector, Dot) {
    ME::Math::Vec3 vec1{1.0f, 2.0f, 3.0f};
    ME::Math::Vec3 vec2{4.0f, -5.0f, 6.0f};
    float f = ME::Math::Vec3::Dot(vec1, vec2);
    EXPECT_EQ(f == 12.0f);
}
TEST(Vector, Cross) {
    ME::Math::Vec3 a{1.0f, 0.0f, 0.0f};
    ME::Math::Vec3 b{0.0f, 1.0f, 0.0f};
    ME::Math::Vec3 c = ME::Math::Vec3::Cross(a, b);
    EXPECT_EQ(c.x == 0.0f);
    EXPECT_EQ(c.y == 0.0f);
    EXPECT_EQ(c.z == 1.0f);
}

TEST(Vector4, One) {
    ME::Math::Vec4 vec4 = ME::Math::Vec4::One;
    EXPECT_EQ(vec4.x == 1.0f);
    EXPECT_EQ(vec4.y == 1.0f);
    EXPECT_EQ(vec4.z == 1.0f);
    EXPECT_EQ(vec4.w == 1.0f);
}

TEST(Vector4, Norm) {
    ME::Math::Vec4 vec4{3.0f, 4.0f, 0.0f, 0.0f};
    vec4.Normalise();
    EXPECT_EQ((vec4 == ME::Math::Vec4{0.6f, 0.8f, 0.0f, 0.0f}));
}

TEST(Vector4, Dot) {
    ME::Math::Vec4 vec1{1.0f, 2.0f, 3.0f, 1.0};
    ME::Math::Vec4 vec2{4.0f, -5.0f, 6.0f, 1.0f};
    float f = ME::Math::Vec4::Dot(vec1, vec2);
    EXPECT_EQ(f == 13.0f);
}

TEST(Vector3i, Up) {
    ME::Math::Vec3i vec3i = ME::Math::Vec3i::Up;
    EXPECT_EQ(vec3i.x == 0);
    EXPECT_EQ(vec3i.y == 1);
    EXPECT_EQ(vec3i.z == 0);
}

TEST(Vector3i, Length) {
    ME::Math::Vec3i vec3i{3, 4, 0};
    float f = vec3i.Length();
    EXPECT_EQ(f == 5.0f);
}

TEST(Vector3i, Dot) {
    ME::Math::Vec3i vec1{1, 2, 3};
    ME::Math::Vec3i vec2{4, -5, 6};
    int i = ME::Math::Vec3i::Dot(vec1, vec2);
    EXPECT_EQ(i == 12);
}