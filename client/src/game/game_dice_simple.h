#pragma once

/**
 * Simple Dice Game..
 */

#include "../scene/scene_dice_simple.h"
#include "game.h"
#include "src/random/random_engine.h"

namespace ME {

class GameDiceSimple : public Game {
   public:
    GameDiceSimple();
    ~GameDiceSimple();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

   private:
    ME::SceneDiceSimple* diceScene = nullptr;  // Scene for the game.
    ME::PhysicsScene* physicsScene = nullptr;  // Physics scene for the game.

    uint32_t frameCounter = 0;
    uint32_t updateIntervalFrames = 16;

    uint32_t frameCounter2 = 0;
    uint32_t updateIntervalFrames2 = 160;

    uint32_t whiteDiceStartAtlasIndex = 777;  // Starting index for white dice textures in the atlas.
    uint32_t blackDiceStartAtlasIndex = 728;  // Alternate starting index for black dice textures in the atlas.
    uint8_t whiteDiceAnimCounter = 0;
    uint8_t blackDiceAnimCounter = 0;

    ME::Random rndWhite{"White", true};
    ME::Random rndBlack{"Black", true};
    ME::Random rndAnimWhite{"WhiteAnim", true};
    ME::Random rndAnimBlack{"BlackAnim", true};

    bool bRolling = false;

    const char* roll = "   Roll   ";
    const char* rolling = "Rolling...";
    const char* resultWaiting = "        ";
    const char* resultWin = "YOU WIN!";
    const char* resultLose = "YOU LOSE";
    const char* resultDraw = "  DRAW  ";
};

}  // namespace ME
