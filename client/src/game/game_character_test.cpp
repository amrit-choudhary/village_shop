#include "game_character_test.h"

ME::GameCharacterTest::GameCharacterTest() : Game() {}

ME::GameCharacterTest::~GameCharacterTest() {}

void ME::GameCharacterTest::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneCharacterTest();
    scene->Init();
    charScene = dynamic_cast<ME::SceneCharacterTest*>(scene);
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    // Init first frame of character animation.
    charScene->spriteInstanceData[0]->atlasIndex =
        charScene->instancedSpriteRenderers[0]->animator->GetCurrentAtlasIndex();

    ME::Log("Character Animation Test Game Start!");
}

void ME::GameCharacterTest::Update(double deltaTime) {
    Game::Update(deltaTime);

    const float speed = 20.0f * deltaTime;
    ME::Vec3 movementVector = ME::Vec3{0.0f, 0.0f, 0.0f};

    ME::SpriteAnimator* animator = charScene->instancedSpriteRenderers[0]->animator;
    bool bAnyKeyDown = false;

    if (inputManager->GetKeyDown(ME::Input::KeyCode::W)) {
        movementVector.y += speed;

        if (animator->GetCurrentClipIndex() != 0) {
            animator->ChangeClip(0);  // Up animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        movementVector.y -= speed;

        if (animator->GetCurrentClipIndex() != 2) {
            animator->ChangeClip(2);  // Down animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        movementVector.x -= speed;

        if (animator->GetCurrentClipIndex() != 3) {
            animator->ChangeClip(3);  // Left animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        movementVector.x += speed;

        if (animator->GetCurrentClipIndex() != 1) {
            animator->ChangeClip(1);  // Right animation
        }
        bAnyKeyDown = true;
    }

    if (bAnyKeyDown) {
        charScene->instancedSpriteRenderers[0]->animator->Update(deltaTime);
        charScene->spriteInstanceData[0]->atlasIndex =
            charScene->instancedSpriteRenderers[0]->animator->GetCurrentAtlasIndex();
    }

    ME::Vec3 currentPosition = charScene->instancedSpriteTransforms[0]->GetPosition() + movementVector;
    charScene->instancedSpriteTransforms[0]->SetPosition(currentPosition);
    charScene->spriteInstanceData[0]->modelMatrixData =
        charScene->instancedSpriteTransforms[0]->GetModelMatrix().GetDataForShader();
}

void ME::GameCharacterTest::End() {
    delete physicsScene;
    physicsScene = nullptr;
    delete charScene;
    charScene = nullptr;

    Game::End();
    ME::Log("Character Animation Test Game End!");
}
