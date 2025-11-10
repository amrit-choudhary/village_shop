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
    CSVParser::Parse(&levelData, "levels/tilemap_03.csv", true);

    for (size_t i = 0; i < levelData.GetTotalCellCount(); ++i) {
        uint32_t tileIndex = levelData.GetValue(i);
        ME::TileRenderData tileRenderData = ME::TileRenderData::FromGlobalTileID(tileIndex);
        rpgScene->spriteInstanceData[i]->atlasIndex = tileRenderData.atlasIndex;
        // Set flags for flipping at bit 0 horizontal and bit 1 vertical.
        rpgScene->spriteInstanceData[i]->flags = (tileRenderData.flipX ? 0x1 : 0x0) |
                                                 (tileRenderData.flipY ? 0x2 : 0x0) |
                                                 (tileRenderData.flipDiagonal ? 0x4 : 0x0);
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
    const float zoomSpeed = 20.0f * deltaTime;

    if (inputManager->GetKeyDown(ME::Input::KeyCode::W)) {
        movementVector.y += speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        movementVector.y -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        movementVector.x -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        movementVector.x += speed;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::UArrow)) {
        scene->spriteCamera->orthographicSize -= zoomSpeed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::DArrow)) {
        scene->spriteCamera->orthographicSize += zoomSpeed;
    }

    scene->spriteCamera->position += movementVector;
    scene->spriteCamera->viewPosition += movementVector;
}

void ME::GameRPG::End() {
    delete scene;
    delete physicsScene;

    Game::End();
}
