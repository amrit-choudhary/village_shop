/**
 * Material class definition.
 * This class represents a material in a 3D rendering engine.
 * It contains properties such as color, texture, and shader information.
 */

#pragma once

#include "color.h"
#include "src/math/vec3.h"

namespace ME {

class Material {
   public:
    Material();
    ~Material();

    Material(const char* texturePath, const char* shaderPath, const ME::Color& color)
        : texturePath(texturePath), shaderPath(shaderPath), color(color) {}
    Material(const char* texturePath, const char* shaderPath)
        : texturePath(texturePath), shaderPath(shaderPath), color(ME::Color::White()) {}

   protected:
    const char* texturePath;
    const char* shaderPath;
    ME::Color color;
};

}  // namespace ME