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
    CSVParser::Parse(&levelData, "levels/tilemap_04.csv", true);

    for (size_t i = 0; i < levelData.GetTotalCellCount(); ++i) {
        uint32_t tileIndex = levelData.GetValue(i);
        ME::TileRenderData tileRenderData = ME::TileRenderData::FromGlobalTileID(tileIndex);
        rpgScene->spriteInstanceData0[i].atlasIndex = tileRenderData.atlasIndex;
        if (tileRenderData.flipDiagonal) {
            if (tileRenderData.flipY) {
                rpgScene->instancedSpriteTransforms0[i]->SetRotation(0.0f, 0.0f, -ME::PI / 2.0f);
                tileRenderData.flipY = false;
            } else {
                rpgScene->instancedSpriteTransforms0[i]->SetRotation(0.0f, 0.0f, ME::PI / 2.0f);
            }
            rpgScene->spriteInstanceData0[i].modelMatrixData =
                rpgScene->instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
        }
        // Set flags for flipping at bit 0 horizontal, bit 1 vertical and bit 2 is diagonal.
        rpgScene->spriteInstanceData0[i].flags = (tileRenderData.flipX ? 0x1 : 0x0) |
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
