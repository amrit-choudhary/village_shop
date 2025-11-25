#include "game_character_test.h"

#include "../scene/scene_ui_hud.h"
#include "physics_scene_char.h"

ME::GameCharacterTest::GameCharacterTest() : Game() {}

ME::GameCharacterTest::~GameCharacterTest() {}

void ME::GameCharacterTest::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneCharacterTest();
    scene->Init();
    charScene = dynamic_cast<ME::SceneCharacterTest*>(scene);
    uiScene = new ME::SceneUIHUD();
    uiScene->Init();

    physicsScene = new ME::PhysicsSceneChar();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    animationSystem->SetScene(scene);
    animationSystem->Init();

    playerTransform = charScene->spriteTransforms[0];
    uiScene->textRenderers[0]->SetText("Bullet Test");
    uiScene->textRenderers[1]->SetText("Score:00000");
    uiScene->textRenderers[2]->SetText("Health:100");

    bulletDirs = new ME::Vec2[maxBulletCount]{};

    ME::Log("Character Animation Test Game Start!");
}

void ME::GameCharacterTest::Update(double deltaTime) {
    Game::Update(deltaTime);

    const float speed = 30.0f * deltaTime;
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
        bulletStartDirAngle = ME::HALF_PI;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        movementVector.x = 0;
        movementVector.y -= speed;

        if (animator->GetCurrentClipIndex() != 2) {
            animator->ChangeClip(2);  // Down animation
        }
        bAnyKeyDown = true;
        bulletStartDirAngle = 3.0f * ME::HALF_PI;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        movementVector.x -= speed;
        movementVector.y = 0;

        if (animator->GetCurrentClipIndex() != 3) {
            animator->ChangeClip(3);  // Left animation
        }
        bAnyKeyDown = true;
        bulletStartDirAngle = 2.0f * ME::HALF_PI;
    }

    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        movementVector.x += speed;
        movementVector.y = 0;

        if (animator->GetCurrentClipIndex() != 1) {
            animator->ChangeClip(1);  // Right animation
        }
        bAnyKeyDown = true;
        bulletStartDirAngle = 0.0f;
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
            --health;

            char healthText[32];
            snprintf(healthText, sizeof(healthText), "Health:%03u", health);
            uiScene->textRenderers[2]->SetText(healthText);
        } else {
            // Move towards player.
            ME::Vec3 moveDir = dirToPlayer.Normalised();
            float speed = enemyBaseSpeed + (i / static_cast<float>(maxNPCCount)) * enemySpeedVariance;
            ME::Vec3 newPos = npcTransform->GetPosition() + moveDir * speed * static_cast<float>(deltaTime);
            npcTransform->SetPosition(newPos);
            charScene->instancedSpriteRenderers0[i]->bDirty = true;
        }

        charScene->dynamicColliders[i].UpdateTransform(*npcTransform, charScene->enemyCollScaleMult);
    }

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score:%05u", score);
    uiScene->textRenderers[1]->SetText(scoreText);

    ME::Vec2 mousePosNorm = inputManager->GetMousePosNorm();
    ME::Vec2 mousePos = mousePosNorm - ME::Vec2{0.5f, 0.5f};
    float inputAngle = atan2(mousePos.y, mousePos.x);

    // Bullet handling.
    ++frameCounter;
    if (frameCounter >= fireRate && inputManager->GetKeyDown(ME::Input::KeyCode::Space)) {
        frameCounter = 0;
        burstCounter = 0;
        ++cycleCounter;
        for (size_t i = 0; i < maxBulletCount; ++i) {
            if (burstCounter >= burstCount) {
                break;
            }
            if (bulletDirs[i].Length() < 1.0f) {
                ++burstCounter;
                float angle = inputAngle + (rndBullet.NextDouble() - 0.5f) * ME::QUARTER_PI * 0.5f;
                bulletDirs[i] = ME::Vec2{cos(angle), sin(angle)};
                charScene->instancedSpriteTransforms1[i]->SetPosition(playerTransform->GetPosition());
                charScene->instancedSpriteRenderers1[i]->atlasIndex = (cycleCounter % 4);
            }
        }
    }

    // Move bullets.
    for (size_t i = 0; i < maxBulletCount; ++i) {
        ME::Transform* bulletTransform = charScene->instancedSpriteTransforms1[i];
        ME::Vec3 bulletPos = bulletTransform->GetPosition();
        bulletPos.x += bulletDirs[i].x * bulletSpeed * static_cast<float>(deltaTime);
        bulletPos.y += bulletDirs[i].y * bulletSpeed * static_cast<float>(deltaTime);

        if ((bulletPos - playerTransform->GetPosition()).Length() > 40.0f) {
            bulletPos = bulletParkPos;
            bulletDirs[i] = ME::Vec2{0.0f, 0.0f};
        }

        bulletTransform->SetPosition(bulletPos);
        charScene->instancedSpriteRenderers1[i]->bDirty = true;

        charScene->dynamicColliders[i].UpdateTransform(*bulletTransform, charScene->bulletCollScaleMult);
    }
}

void ME::GameCharacterTest::End() {
    delete physicsScene;
    physicsScene = nullptr;
    delete charScene;
    charScene = nullptr;

    delete[] bulletDirs;
    bulletDirs = nullptr;

    Game::End();
    ME::Log("Character Animation Test Game End!");
}

void ME::GameCharacterTest::CollisionCallback(ColliderAABB* a, ColliderAABB* b, CollisionResultAABB* result) {
    uint32_t enemeyIndex = b->GetID();
    ME::Transform* npcTransform = charScene->instancedSpriteTransforms0[enemeyIndex];
    ME::Vec3 dirToPlayer = playerTransform->GetPosition() - npcTransform->GetPosition();

    ME::Vec3 throwDir = dirToPlayer.Normalised();
    ME::Vec3 newPos = npcTransform->GetPosition() - throwDir * outThrowDistance;
    npcTransform->SetPosition(newPos);
    charScene->instancedSpriteRenderers0[enemeyIndex]->bDirty = true;
    ++score;

    delete result;
}
