#include "game_character_test.h"

ME::GameCharacterTest::GameCharacterTest() : Game() {}

ME::GameCharacterTest::~GameCharacterTest() {}

void ME::GameCharacterTest::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneCharacterTest();
    scene->Init();
    charScene = dynamic_cast<ME::SceneCharacterTest*>(scene);
    uiScene = new ME::SceneUI();
    uiScene->Init();
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);
    animationSystem->SetScene(scene);
    animationSystem->Init();

    playerTransform = charScene->spriteTransforms[0];
    uiScene->textRenderers[0]->SetText("  Character Test  ");

    ME::Log("Character Animation Test Game Start!");
}

void ME::GameCharacterTest::Update(double deltaTime) {
    Game::Update(deltaTime);

    const float speed = 20.0f * deltaTime;
    ME::Vec3 movementVector = ME::Vec3{0.0f, 0.0f, 0.0f};

    ME::SpriteAnimator* animator = charScene->spriteRenderers[0]->animator;
    bool bAnyKeyDown = false;

    if (inputManager->GetKeyDown(ME::Input::KeyCode::W)) {
        movementVector.x = 0;
        movementVector.y += speed;

        if (animator->GetCurrentClipIndex() != 0) {
            animator->ChangeClip(0);  // Up animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        movementVector.x = 0;
        movementVector.y -= speed;

        if (animator->GetCurrentClipIndex() != 2) {
            animator->ChangeClip(2);  // Down animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        movementVector.x -= speed;
        movementVector.y = 0;

        if (animator->GetCurrentClipIndex() != 3) {
            animator->ChangeClip(3);  // Left animation
        }
        bAnyKeyDown = true;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        movementVector.x += speed;
        movementVector.y = 0;

        if (animator->GetCurrentClipIndex() != 1) {
            animator->ChangeClip(1);  // Right animation
        }
        bAnyKeyDown = true;
    }

    if (bAnyKeyDown) {
        animator->StartAnimation();
    } else {
        animator->StopAnimation();
    }

    ME::Vec3 currentPosition = playerTransform->GetPosition() + movementVector;
    playerTransform->SetPosition(currentPosition);
    charScene->spriteRenderers[0]->bDirty = true;
    charScene->spriteCamera->position += ME::Vec3{movementVector.x, movementVector.y, 0.0f};
    charScene->spriteCamera->viewPosition += ME::Vec3{movementVector.x, movementVector.y, 0.0f};

    if (inputManager->GetKeyPressed(ME::Input::KeyCode::Space)) {
        ME::Log("Space key pressed Down!");
    }

    if (inputManager->GetKeyReleased(ME::Input::KeyCode::Space)) {
        ME::Log("Space key released!");
    }

    for (int i = 0; i < maxNPCCount; ++i) {
        ME::Transform* npcTransform = charScene->instancedSpriteTransforms0[i];
        ME::Vec3 dirToPlayer = playerTransform->GetPosition() - npcTransform->GetPosition();
        float distSqr = dirToPlayer.Length();

        if (distSqr < minDistaneToPlayerSqr) {
            // Teleport NPC away.
            ME::Vec3 throwDir = dirToPlayer.Normalised();
            ME::Vec3 newPos = npcTransform->GetPosition() - throwDir * outThrowDistance;
            npcTransform->SetPosition(newPos);
            charScene->instancedSpriteRenderers0[i]->bDirty = true;
            ++score;
        } else {
            // Move towards player.
            ME::Vec3 moveDir = dirToPlayer.Normalised();
            float speed = enemyBaseSpeed + (i / static_cast<float>(maxNPCCount)) * enemySpeedVariance;
            ME::Vec3 newPos = npcTransform->GetPosition() + moveDir * speed * static_cast<float>(deltaTime);
            npcTransform->SetPosition(newPos);
            charScene->instancedSpriteRenderers0[i]->bDirty = true;
        }
    }

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score:%05u", score);
    uiScene->textRenderers[1]->SetText(scoreText);

    const size_t maxBulletCount = 9'000;
    if (inputManager->GetKeyReleased(ME::Input::KeyCode::Space)) {
        for (int i = 0; i < maxBulletCount; ++i) {
            charScene->instancedSpriteRenderers1[i]->color = ME::Color::RandomColorPretty();
            charScene->instancedSpriteRenderers1[i]->bDirty = true;
        }
    }
}

void ME::GameCharacterTest::End() {
    delete physicsScene;
    physicsScene = nullptr;
    delete charScene;
    charScene = nullptr;

    Game::End();
    ME::Log("Character Animation Test Game End!");
}
