#include "game_rpg.h"

#include "../world/tile.h"
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
    CSVParser::Parse(&levelData, "levels/tilemap_01.csv", true);

    for (size_t i = 0; i < levelData.GetTotalCellCount(); ++i) {
        uint32_t tileIndex = levelData.GetValue(i);
        ME::TileRenderData tileRenderData = ME::TileRenderData::FromGlobalTileID(tileIndex);
        rpgScene->spriteInstanceData[i]->atlasIndex = tileRenderData.atlasIndex;
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

    const float speed = 40.0f * deltaTime;
    ME::Vec3 movementVector = ME::Vec3{0.0f, 0.0f, 0.0f};

    if (inputManager->GetKeyDown(ME::Input::KeyCode::W) || inputManager->GetKeyDown(ME::Input::KeyCode::UArrow)) {
        movementVector.y += speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::S) || inputManager->GetKeyDown(ME::Input::KeyCode::DArrow)) {
        movementVector.y -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::A) || inputManager->GetKeyDown(ME::Input::KeyCode::LArrow)) {
        movementVector.x -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::D) || inputManager->GetKeyDown(ME::Input::KeyCode::RArrow)) {
        movementVector.x += speed;
    }

    scene->spriteCamera->position += movementVector;
    scene->spriteCamera->viewPosition += movementVector;
}

void ME::GameRPG::End() {
    delete scene;
    delete physicsScene;

    Game::End();
}
