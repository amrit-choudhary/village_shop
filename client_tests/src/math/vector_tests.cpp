#include "vector_tests.h"

#include <iostream>

bool TEST::Test_Vec31() {
    std::cout << "\n\nTEST: Starting: Vec3 Test = static constants" << '\n';

    ME::Math::Vec3 vec3 = ME::Math::Vec3::Up;

    if (vec3.x == 0.0f && vec3.y == 1.0f && vec3.z == 0.0f) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}
bool TEST::Test_Vec32() {
    std::cout << "\n\nTEST: Starting: Vec3 Test = Normalise." << '\n';

    ME::Math::Vec3 vec3{3.0f, 4.0f, 0.0f};
    vec3.Normalise();

    if (vec3.x == 0.6f && vec3.y == 0.8f && vec3.z == 0.0f) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}
bool TEST::Test_Vec33() {
    std::cout << "\n\nTEST: Starting: Vec3 Test = Dot" << '\n';

    ME::Math::Vec3 vec1{1.0f, 2.0f, 3.0f};
    ME::Math::Vec3 vec2{4.0f, -5.0f, 6.0f};
    float f = ME::Math::Vec3::Dot(vec1, vec2);

    if (f == 12.0f) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}
bool TEST::Test_Vec34() {
    std::cout << "\n\nTEST: Starting: Vec3 Test = Cross." << '\n';

    ME::Math::Vec3 a{1.0f, 0.0f, 0.0f};
    ME::Math::Vec3 b{0.0f, 1.0f, 0.0f};
    ME::Math::Vec3 c = ME::Math::Vec3::Cross(a, b);

    if (c.x == 0.0f && c.y == 0.0f && c.z == 1.0f) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}