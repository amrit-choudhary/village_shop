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

static bool RunDXCCompile(const std::filesystem::path& src, const std::filesystem::path& includeDir,
                          const std::string& target, const std::string& entry, const std::filesystem::path& outPath) {
    const std::string dxc = "dxc.exe";
    std::string cmd = dxc + " -T " + target + " -E " + entry + " -Fo \"" + outPath.string() + "\"" + " -O3 -I \"" +
                      includeDir.string() + "\" \"" + src.string() + "\"";
    int rc = std::system(cmd.c_str());
    return rc == 0 && std::filesystem::exists(outPath);
}

static bool RunTexconv(const std::filesystem::path& src, const std::filesystem::path& outDir) {
    const std::string texconv = "texconv.exe";
    std::string cmd =
        texconv + " -f R8G8B8A8_UNORM -m 1 -ft DDS -y -o \"" + outDir.string() + "\" \"" + src.string() + "\"";
    int rc = std::system(cmd.c_str());
    return (rc == 0);
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
        // Original hlsl shader path in client project.
        std::filesystem::path src = GetResPathSource(fullShaderPath);
        std::filesystem::path includeDir = src.parent_path();
        std::filesystem::path dest = GetResPathDest(fullShaderPath);
        std::filesystem::path destDir = dest.parent_path();

        if (!std::filesystem::exists(destDir)) {
            std::filesystem::create_directories(destDir);
        }

        std::string stem = src.stem().string();
        std::filesystem::path vsOutPath = destDir / (stem + "_vs.cso");
        std::filesystem::path psOutPath = destDir / (stem + "_ps.cso");

        bool bVSCompiled = RunDXCCompile(src, includeDir, "vs_6_0", "VS", vsOutPath);
        bool bPSCompiled = RunDXCCompile(src, includeDir, "ps_6_0", "PS", psOutPath);

        if (!bVSCompiled || !bPSCompiled) {
            std::cerr << "Failed to compile shader: " << shaderPath << '\n';
            return false;
        }
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
        std::filesystem::path src = GetResPathSource(fullTexturePath);
        std::filesystem::path dest = GetResPathDest(fullTexturePath);
        std::filesystem::path destDir = dest.parent_path();

        if (!std::filesystem::exists(destDir)) {
            std::filesystem::create_directories(destDir);
        }

        bool bTexConverted = RunTexconv(src, destDir);
        if (!bTexConverted) {
            std::cerr << "Failed to convert texture: " << texturePath << '\n';
            return false;
        }
    }

    std::cout << "Successfully packaged Client for Windows." << '\n';
    return true;
}

#endif  // VG_WIN
