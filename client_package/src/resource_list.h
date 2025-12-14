#pragma once

/**
 * Resource listing for packaging.
 */

#include <string>
#include <vector>

namespace ME::Package {

class ResourceList {
   public:
    ResourceList() = default;
    ~ResourceList() = default;

    std::vector<std::string> anim;
    std::vector<std::string> audio;
    std::vector<std::string> config;
    std::vector<std::string> game_data;
    std::vector<std::string> shaders;
    std::vector<std::string> texture_data;
    std::vector<std::string> textures;
};

}  // namespace ME::Package
