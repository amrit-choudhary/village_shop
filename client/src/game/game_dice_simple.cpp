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
    animationSystem->SetScene(scene);
    animationSystem->Init();

    ME::Log("Simple Dice Game Start!");
}

void ME::GameDiceSimple::Update(double deltaTime) {
    Game::Update(deltaTime);

    frameCounter++;
    if (frameCounter > updateIntervalFrames) {
        frameCounter = 0;
        // Update game logic here.
        if (bRolling) {
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
            diceScene->instancedSpriteRenderers[0]->animator->StartAnimation();
            diceScene->instancedSpriteRenderers[1]->animator->StartAnimation();
        } else {
            // Decide result
            uint8_t whiteDiceAnimCounter = rndAnimWhite.NextRange(0, 5);
            uint8_t blackDiceAnimCounter = rndAnimBlack.NextRange(0, 5);

            diceScene->textRenderers[1]->SetText(roll);
            if (whiteDiceAnimCounter > blackDiceAnimCounter) {
                diceScene->textRenderers[2]->SetText(resultWin);
            } else if (whiteDiceAnimCounter < blackDiceAnimCounter) {
                diceScene->textRenderers[2]->SetText(resultLose);
            } else {
                diceScene->textRenderers[2]->SetText(resultDraw);
            }
            diceScene->instancedSpriteRenderers[0]->animator->StopAnimation();
            diceScene->instancedSpriteRenderers[1]->animator->StopAnimation();
        }
    }
}

void ME::GameDiceSimple::End() {
    delete scene;
    delete physicsScene;

    Game::End();
}
