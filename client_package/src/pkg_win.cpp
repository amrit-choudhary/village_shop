#ifdef VG_WIN

#include "pkg_win.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "resource_list.h"
#include "third_party/json/cJSON.h"

static cJSON* LoadJSONFromFile(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        const std::string fileContent = buffer.str();

        cJSON* json = cJSON_Parse(fileContent.c_str());
        if (!json) {
            std::cout << "Failed to parse JSON: " + std::string(cJSON_GetErrorPtr()) << std::endl;
            return nullptr;
        }
        return json;
    } else {
        std::cout << "Unable to open file" << std::endl;
        return nullptr;
    }
    return nullptr;
}

static bool parseArrayToVector(cJSON* root, const char* name, std::vector<std::string>& dst) {
    cJSON* arr = cJSON_GetObjectItemCaseSensitive(root, name);
    if (!arr || !cJSON_IsArray(arr)) return false;
    cJSON* item = nullptr;
    cJSON_ArrayForEach(item, arr) {
        if (cJSON_IsString(item) && item->valuestring) {
            dst.emplace_back(item->valuestring);
        }
    }
    return true;
}

bool ME::Package::PackageClientWin(const std::string& exePath, const std::string& buildPath) {
    std::cout << "Packaging Client for Windows" << '\n';

    std::filesystem::path exeFullPath = std::filesystem::absolute(exePath);
    std::filesystem::path exeDir = exeFullPath.parent_path();
    std::filesystem::path resourceDirPath = exeDir / "resources";
    std::filesystem::path resListPath = resourceDirPath / "res_list_win.json";

    cJSON* root = LoadJSONFromFile(resListPath.string().c_str());
    if (!root) {
        std::cerr << "Failed to load resource list JSON." << '\n';
        return false;
    }

    ResourceList resList;
    parseArrayToVector(root, "anim", resList.anim);
    parseArrayToVector(root, "audio", resList.audio);
    parseArrayToVector(root, "config", resList.config);
    parseArrayToVector(root, "shaders", resList.shaders);
    parseArrayToVector(root, "texture_data", resList.texture_data);
    parseArrayToVector(root, "textures", resList.textures);

    cJSON_Delete(root);

    // Animation Resources
    for (const auto& animPath : resList.anim) {
        std::filesystem::path fullAnimPath = resourceDirPath / "anim" / animPath;
        std::cout << "Packaging Animation: " << fullAnimPath << '\n';
    }

    // Audio Resources
    for (const auto& audioPath : resList.audio) {
        std::filesystem::path fullAudioPath = resourceDirPath / "audio" / audioPath;
        std::cout << "Packaging Audio: " << fullAudioPath << '\n';
    }

    // Config Resources
    for (const auto& configPath : resList.config) {
        std::filesystem::path fullConfigPath = resourceDirPath / "config" / configPath;
        std::cout << "Packaging Config: " << fullConfigPath << '\n';
    }

    // Shader Resources
    for (const auto& shaderPath : resList.shaders) {
        std::filesystem::path fullShaderPath = resourceDirPath / "shaders" / shaderPath;
        std::cout << "Packaging Shader: " << fullShaderPath << '\n';
    }

    // Texture Data Resources
    for (const auto& texDataPath : resList.texture_data) {
        std::filesystem::path fullTexDataPath = resourceDirPath / "texture_data" / texDataPath;
        std::cout << "Packaging Texture Data: " << fullTexDataPath << '\n';
    }

    // Texture Resources
    for (const auto& texturePath : resList.textures) {
        std::filesystem::path fullTexturePath = resourceDirPath / "textures" / texturePath;
        std::cout << "Packaging Texture: " << fullTexturePath << '\n';
    }

    return true;
}

#endif  // VG_WIN
