#include "level_breakout.h"

#include <fstream>
#include <string>

#include "../../../shared/src/misc/utils.h"

ME::Level::Level() : gridX(0), gridY(0), brickCount(0), bricks(nullptr) {}

ME::Level::Level(uint8_t x, uint8_t y, uint16_t count) : gridX(x), gridY(y), brickCount(count) {
    if (count > 0) {
        bricks = new Brick[count];
    } else {
        bricks = nullptr;
    }
}

ME::Level::~Level() {
    delete[] bricks;
}

void ME::Level::Save(const Level& level, const char* filePath) {
    Level level1{};
    level1.gridX = 30;
    level1.gridY = 1;
    level1.brickCount = 256;
    level1.bricks = new Brick[level1.brickCount];
    for (uint16_t i = 0; i < level1.brickCount; ++i) {
        level1.bricks[i].type = static_cast<BrickType>(i % 8);
    }

    std::string fileName = ME::Utils::GetResourcesPath() + filePath;
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for saving level data.");
    }

    file.write(reinterpret_cast<const char*>(&level1.gridX), sizeof(level1.gridX));
    file.write(reinterpret_cast<const char*>(&level1.gridY), sizeof(level1.gridY));
    file.write(reinterpret_cast<const char*>(&level1.brickCount), sizeof(level1.brickCount));

    for (uint16_t i = 0; i < level1.brickCount; ++i) {
        file.write(reinterpret_cast<const char*>(&level1.bricks[i]), sizeof(Brick));
    }

    file.close();
}

void ME::Level::Load(Level* level, const char* filePath) {
    std::string fileName = ME::Utils::GetResourcesPath() + filePath;
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for saving level data.");
    }

    file.read(reinterpret_cast<char*>(&level->gridX), sizeof(level->gridX));
    file.read(reinterpret_cast<char*>(&level->gridY), sizeof(level->gridY));
    file.read(reinterpret_cast<char*>(&level->brickCount), sizeof(level->brickCount));

    level->bricks = new Brick[level->brickCount];

    for (uint16_t i = 0; i < level->brickCount; ++i) {
        file.read(reinterpret_cast<char*>(&level->bricks[i]), sizeof(Brick));
    }

    file.close();
}
