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

static std::filesystem::path GetResPathSource(const std::filesystem::path& p) {
    std::filesystem::path out;
    for (const auto& part : p) {
        const std::string s = part.string();
        if (s == "build") continue;
        if (s == "client_package") {
            out /= "client";
        } else {
            out /= part;
        }
    }
    return out;
}

static std::filesystem::path GetResPathDest(const std::filesystem::path& p) {
    std::filesystem::path out;
    for (const auto& part : p) {
        const std::string s = part.string();
        if (s == "client_package") {
            out /= "client";
        } else {
            out /= part;
        }
    }
    return out;
}

static std::filesystem::path GetPathByReplacingClientPackage(const std::filesystem::path& p) {
    std::filesystem::path out;
    for (const auto& part : p) {
        const std::string s = part.string();
        if (s == "client_package") {
            out /= "client";
        } else
            out /= part;
    }
    return out;
}

static bool CopyFileEnsureDir(const std::filesystem::path& src, const std::filesystem::path& dest) {
    if (!std::filesystem::exists(dest.parent_path())) {
        std::filesystem::create_directories(dest.parent_path());
    }
    std::filesystem::copy_file(src, dest, std::filesystem::copy_options::overwrite_existing);
    return true;
}

bool ME::Package::PackageClientWin(const std::string& exePath, const std::string& buildPath) {
    std::cout << "Packaging Client for Windows" << '\n';

    std::filesystem::path exeFullPath = std::filesystem::absolute(exePath);
    std::filesystem::path exeDir = exeFullPath.parent_path();
    std::filesystem::path resourceDirPath = exeDir / "resources";
    std::filesystem::path resListPath = resourceDirPath / "res_list_win.json";

    std::filesystem::path clientExeDir = GetPathByReplacingClientPackage(exeDir);
    std::filesystem::path clientResDir = clientExeDir / "resources";

    cJSON* root = LoadJSONFromFile(resListPath.string().c_str());
    if (!root) {
        std::cerr << "Failed to load resource list JSON." << '\n';
        return false;
    }

    ResourceList resList;
    parseArrayToVector(root, "anim", resList.anim);
    parseArrayToVector(root, "audio", resList.audio);
    parseArrayToVector(root, "config", resList.config);
    parseArrayToVector(root, "game_data", resList.game_data);
    parseArrayToVector(root, "shaders", resList.shaders);
    parseArrayToVector(root, "texture_data", resList.texture_data);
    parseArrayToVector(root, "textures", resList.textures);

    cJSON_Delete(root);

    if (!std::filesystem::exists(clientResDir)) {
        std::filesystem::create_directories(clientResDir);
    }

    // Animation Resources
    if (!std::filesystem::exists(clientResDir / "anim")) {
        std::filesystem::create_directories(clientResDir / "anim");
    }

    for (const auto& animPath : resList.anim) {
        std::filesystem::path fullAnimPath = resourceDirPath / "anim" / animPath;
        CopyFileEnsureDir(GetResPathSource(fullAnimPath), GetResPathDest(fullAnimPath));
    }

    // Audio Resources
    if (!std::filesystem::exists(clientResDir / "audio")) {
        std::filesystem::create_directories(clientResDir / "audio");
    }

    for (const auto& audioPath : resList.audio) {
        std::filesystem::path fullAudioPath = resourceDirPath / "audio" / audioPath;
        CopyFileEnsureDir(GetResPathSource(fullAudioPath), GetResPathDest(fullAudioPath));
    }

    // Config Resources
    if (!std::filesystem::exists(clientResDir / "config")) {
        std::filesystem::create_directories(clientResDir / "config");
    }

    for (const auto& configPath : resList.config) {
        std::filesystem::path fullConfigPath = resourceDirPath / "config" / configPath;
        CopyFileEnsureDir(GetResPathSource(fullConfigPath), GetResPathDest(fullConfigPath));
    }

    // Game Data Resources
    if (!std::filesystem::exists(clientResDir / "game_data")) {
        std::filesystem::create_directories(clientResDir / "game_data");
    }

    for (const auto& gameDataPath : resList.game_data) {
        std::filesystem::path fullGameDataPath = resourceDirPath / "game_data" / gameDataPath;
        CopyFileEnsureDir(GetResPathSource(fullGameDataPath), GetResPathDest(fullGameDataPath));
    }

    // Shader Resources
    if (!std::filesystem::exists(clientResDir / "shaders")) {
        std::filesystem::create_directories(clientResDir / "shaders");
    }

    for (const auto& shaderPath : resList.shaders) {
        std::filesystem::path fullShaderPath = resourceDirPath / "shaders" / shaderPath;
        CopyFileEnsureDir(GetResPathSource(fullShaderPath), GetResPathDest(fullShaderPath));
    }

    // Texture Data Resources
    if (!std::filesystem::exists(clientResDir / "texture_data")) {
        std::filesystem::create_directories(clientResDir / "texture_data");
    }

    for (const auto& texDataPath : resList.texture_data) {
        std::filesystem::path fullTexDataPath = resourceDirPath / "texture_data" / texDataPath;
        CopyFileEnsureDir(GetResPathSource(fullTexDataPath), GetResPathDest(fullTexDataPath));
    }

    // Texture Resources
    if (!std::filesystem::exists(clientResDir / "textures")) {
        std::filesystem::create_directories(clientResDir / "textures");
    }

    for (const auto& texturePath : resList.textures) {
        std::filesystem::path fullTexturePath = resourceDirPath / "textures" / texturePath;
        CopyFileEnsureDir(GetResPathSource(fullTexturePath), GetResPathDest(fullTexturePath));
    }

    std::cout << "Successfully packaged Client for Windows." << '\n';
    return true;
}

#endif  // VG_WIN
