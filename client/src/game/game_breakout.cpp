#include "game_breakout.h"

ME::GameBreakout::GameBreakout() : Game() {}

ME::GameBreakout::~GameBreakout() {}

void ME::GameBreakout::Init(ME::Time::TimeManager *currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneBreakout();
    scene->Init();
    brkScene = dynamic_cast<ME::SceneBreakout *>(scene);
    ballTransform = brkScene->instancedSpriteTransforms[brkScene->ballIndex];
    ballInstanceData = brkScene->spriteInstanceData[brkScene->ballIndex];
    ballVelocity = brkScene->ballVelocity;

    // Initialize the physics scene for Breakout.
    physicsScene = new ME::PhysicsSceneBreakout();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    ballCollider = &(physicsScene->dynamicColliders[0]);
    ballIndex = ballCollider->GetID();

    ME::Log("Breakout Game Start!");
}

void ME::GameBreakout::Update(double deltaTime) {
    Game::Update(deltaTime);
    ballTransform->SetPosition(ballTransform->GetPosition().x + brkScene->ballSpeed * ballVelocity.x * deltaTime,
                               ballTransform->GetPosition().y + brkScene->ballSpeed * ballVelocity.y * deltaTime, 0.0f);
    ballInstanceData->modelMatrixData = ballTransform->GetModelMatrix().GetData();

    if (ballTransform->GetPosition().x < brkScene->originX ||
        ballTransform->GetPosition().x > brkScene->originX + (brkScene->gridX * brkScene->brickWidth)) {
        ballVelocity.x *= -1.0f;  // Bounce off the walls.
    }
    if (ballTransform->GetPosition().y < brkScene->originY ||
        ballTransform->GetPosition().y > brkScene->originY + (brkScene->gridY * brkScene->brickHeight)) {
        ballVelocity.y *= -1.0f;  // Bounce off the top and bottom.
    }

    ballCollider->UpdateTransform(*ballTransform, brkScene->ballCollScaleMult);
}

void ME::GameBreakout::End() {
    Game::End();
    delete scene;
    delete physicsScene;
}

void ME::GameBreakout::CollisionCallback(ME::ColliderAABB *a, ME::ColliderAABB *b) {
    uint32_t ballIndex = brkScene->ballIndex;
    if (a->GetID() == ballIndex) {
        if (IsDestructible(b->GetID())) {
            b->isEnabled = false;
            brkScene->spriteInstanceData[b->GetID()]->atlasIndex = 0;  // Set to black if hit.
        }
    }

    if (b->GetID() == ballIndex) {
        if (IsDestructible(a->GetID())) {
            a->isEnabled = false;
            brkScene->spriteInstanceData[a->GetID()]->atlasIndex = 0;  // Set to black if hit.
        }
    }
}

bool ME::GameBreakout::IsDestructible(uint32_t index) const {
    if (index < brkScene->gridCount) {
        return brkScene->spriteInstanceData[index]->atlasIndex != 0;
    }

    return false;
}
