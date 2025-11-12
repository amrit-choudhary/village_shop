#include "game_dice_simple.h"

ME::GameDiceSimple::GameDiceSimple() : Game() {}

ME::GameDiceSimple::~GameDiceSimple() {}

void ME::GameDiceSimple::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneDiceSimple();
    scene->Init();
    diceScene = dynamic_cast<ME::SceneDiceSimple*>(scene);
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    whiteDiceAnimCounter = 0;
    blackDiceAnimCounter = 0;
    diceScene->spriteInstanceData[0]->atlasIndex = whiteDiceStartAtlasIndex + whiteDiceAnimCounter;
    diceScene->spriteInstanceData[1]->atlasIndex = blackDiceStartAtlasIndex + blackDiceAnimCounter;

    ME::Log("Simple Dice Game Start!");
}

void ME::GameDiceSimple::Update(double deltaTime) {
    Game::Update(deltaTime);

    frameCounter++;
    if (frameCounter > updateIntervalFrames) {
        frameCounter = 0;
        // Update game logic here.
        if (bRolling) {
            whiteDiceAnimCounter = rndAnimWhite.NextRange(0, 5);
            blackDiceAnimCounter = rndAnimBlack.NextRange(0, 5);
            diceScene->spriteInstanceData[0]->atlasIndex = whiteDiceStartAtlasIndex + whiteDiceAnimCounter;
            diceScene->spriteInstanceData[1]->atlasIndex = blackDiceStartAtlasIndex + blackDiceAnimCounter;
        }
    }

    frameCounter2++;
    if (frameCounter2 > updateIntervalFrames2) {
        frameCounter2 = 0;
        bRolling = !bRolling;

        if (bRolling) {
            // Start rolling
            diceScene->textRenderers[1]->SetText(rolling);
            diceScene->textRenderers[2]->SetText(resultWaiting);
        } else {
            // Decide result
            diceScene->textRenderers[1]->SetText(roll);
            if (whiteDiceAnimCounter > blackDiceAnimCounter) {
                diceScene->textRenderers[2]->SetText(resultWin);
            } else if (whiteDiceAnimCounter < blackDiceAnimCounter) {
                diceScene->textRenderers[2]->SetText(resultLose);
            } else {
                diceScene->textRenderers[2]->SetText(resultDraw);
            }
        }

        diceScene->UpdateTextInstanceData();
    }
}

void ME::GameDiceSimple::End() {
    delete scene;
    delete physicsScene;

    Game::End();
}
