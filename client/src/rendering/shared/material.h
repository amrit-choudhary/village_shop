/**
 * Material class definition.
 * This class represents a material in a 3D rendering engine.
 * It contains properties such as color, texture, and shader information.
 */

#pragma once

#include <cstdint>

#include "color.h"
#include "src/math/vec3.h"

namespace ME {

// Enum class for depth compare functions
enum class DepthCompareFunction : uint8_t { Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual, Always };

class Material {
   public:
    Material();
    ~Material();

    Material(const char* texturePath, const char* shaderPath, const ME::Color& color)
        : texturePath(texturePath), shaderPath(shaderPath), color(color) {}
    Material(const char* texturePath, const char* shaderPath)
        : texturePath(texturePath), shaderPath(shaderPath), color(ME::Color::White()) {}

    const char* texturePath;
    const char* shaderPath;
    ME::Color color;
    ME::DepthCompareFunction depthCompareFunction = ME::DepthCompareFunction::Less;
    bool depthWriteEnabled = true;
};

}  // namespace ME
