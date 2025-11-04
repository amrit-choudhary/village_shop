#include "game_rpg.h"

#include "src/file_io/csv/csv_parser.h"

ME::GameRPG::GameRPG() : Game() {}

ME::GameRPG::~GameRPG() {}

void ME::GameRPG::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneRPG();
    scene->Init();
    rpgScene = dynamic_cast<ME::SceneRPG*>(scene);
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    CSVData levelData;
    CSVParser::Parse(&levelData, "levels/tilemap_01.csv");

    for (size_t i = 0; i < levelData.GetTotalCellCount(); ++i) {
        uint32_t tileIndex = levelData.GetValue(i);
        if (tileIndex != 0) {
            rpgScene->spriteInstanceData[i]->atlasIndex = tileIndex - 1;
        }
    }

    ME::Log("RPG Game Start!");
}

void ME::GameRPG::Update(double deltaTime) {
    Game::Update(deltaTime);

    frameCounter++;
    if (frameCounter > updateIntervalFrames) {
        frameCounter = 0;
        // Update game logic here.
    }

    scene->spriteCamera->position.x += 8.0f * static_cast<float>(deltaTime);
    scene->spriteCamera->position.y += 3.0f * static_cast<float>(deltaTime);

    scene->spriteCamera->viewPosition.x += 8.0f * static_cast<float>(deltaTime);
    scene->spriteCamera->viewPosition.y += 3.0f * static_cast<float>(deltaTime);
}

void ME::GameRPG::End() {
    delete scene;
    delete physicsScene;

    Game::End();
}
