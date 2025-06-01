/**
 * Data for a single level in the Breakout game.
 * This contains the brick layout, paddle size, ball speed, and other game-specific parameters.
 * This gets saved and loaded from a file.
 * The level is used to initialize the game state when starting a new level.
 */

#pragma once

#include <cstdint>

namespace ME {

// Defines the types of bricks used in the Breakout game.
enum class BrickType : uint8_t {
    Empty = 0x0,  // No brick
    One = 0x1,
    Two = 0x2,
    Three = 0x3,
    Four = 0x4,
    Five = 0x5,
    Six = 0x6,
    Seven = 0x7,
    Eight = 0x8,
    Nine = 0x9
};

class Brick {
   public:
    BrickType type;  // Type of the brick
};

class Level {
   public:
    uint8_t gridX;
    uint8_t gridY;
    uint16_t brickCount;
    Brick* bricks;

    Level();
    Level(uint8_t x, uint8_t y, uint16_t count);
    virtual ~Level();

    static void Save(const Level& level, const char* filePath);
    static void Load(Level* level, const char* filePath);
};

}  // namespace ME
