#include "game_character_test.h"

#include "../scene/scene_ui_hud.h"
#include "../utils/json_utils.h"
#include "physics_scene_char.h"
#include "src/random/random_utils.h"

ME::GameCharacterTest::GameCharacterTest() : Game() {}

ME::GameCharacterTest::~GameCharacterTest() {}

void ME::GameCharacterTest::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);

    charScene = new ME::SceneCharacterTest();
    scene = charScene;
    scene->Init();

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

    ME::JsonUtils::LoadWaveDataFromJSON("game_data/wave_data.json", &waveData);
    enemies = new Enemy[maxNPCCount]{};
    for (size_t i = 0; i < maxNPCCount; ++i) {
        enemies[i].type = 0;
        enemies[i].health = 100;
        enemies[i].maxHealth = 100;
        enemies[i].speed = 10;
        enemies[i].spriteIndex = 0;
        enemies[i].bActive = false;
    }
    SpawnNextEnemy();

    ME::Log("Character Animation Test Game Start!");
}

void ME::GameCharacterTest::Start() {
    Game::Start();

    audioSystem->SetMasterVolume(0.1f);
    audioSystem->PlayMusic(0, true);
}

void ME::GameCharacterTest::Update(double deltaTime) {
    Game::Update(deltaTime);

    const float speed = 30.0f * deltaTime;
    const float zoomSpeed = 20.0f * deltaTime;
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

    float cameraZDelta = 0.0f;

    if (inputManager->GetKeyDown(ME::Input::KeyCode::UpArrow)) {
        charScene->spriteCamera->orthographicSize -= zoomSpeed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::DownArrow)) {
        charScene->spriteCamera->orthographicSize += zoomSpeed;
    }

    charScene->spriteCamera->position += ME::Vec3{movementVector.x, movementVector.y, 0};
    charScene->spriteCamera->viewPosition += ME::Vec3{movementVector.x, movementVector.y, 0.0f};

    for (int i = 0; i < maxNPCCount; ++i) {
        if (!enemies[i].bActive) {
            continue;
        }

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

        audioSystem->PlayAudio(0, false);
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

        charScene->dynamicColliders[(maxNPCCount + i)].UpdateTransform(*bulletTransform,
                                                                       charScene->bulletCollScaleMult);
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
    // a is always enemy, b is always bullet.
    uint32_t enemyIndex = a->GetID();
    uint32_t bulletIndex = b->GetID() - maxNPCCount;

    ME::Transform* npcTransform = charScene->instancedSpriteTransforms0[enemyIndex];
    ME::Vec3 dirToPlayer = playerTransform->GetPosition() - npcTransform->GetPosition();

    ME::Vec3 throwDir = dirToPlayer.Normalised();
    ME::Vec3 newPos = npcTransform->GetPosition() - throwDir * outThrowDistance;
    npcTransform->SetPosition(newPos);
    charScene->instancedSpriteRenderers0[enemyIndex]->bDirty = true;
    charScene->dynamicColliders[enemyIndex].UpdateTransform(*npcTransform, charScene->enemyCollScaleMult);
    ++score;

    ME::Vec3 bulletPos = bulletParkPos;
    bulletDirs[bulletIndex] = ME::Vec2{0.0f, 0.0f};

    ME::Transform* bulletTransform = charScene->instancedSpriteTransforms1[bulletIndex];
    bulletTransform->SetPosition(bulletPos);
    charScene->instancedSpriteRenderers1[bulletIndex]->bDirty = true;
    charScene->dynamicColliders[(maxNPCCount + bulletIndex)].UpdateTransform(*bulletTransform,
                                                                             charScene->bulletCollScaleMult);

    delete result;
}

void ME::GameCharacterTest::SpawnNextEnemy() {
    ME::Random rndPos{"npc_spawn_pos", true};
    for (int i = 0; i < 10; ++i) {
        if (!enemies[i].bActive) {
            enemies[i].bActive = true;
            ME::Vec2 pos = ME::Utils::RandomVec2OnCircle(rndPos) * 100.0f;
            charScene->instancedSpriteTransforms0[i]->SetPosition(ME::Vec3{pos.x, pos.y, 0.0f});
            // return;
        }
    }
}
