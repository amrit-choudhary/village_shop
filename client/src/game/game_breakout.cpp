#include "game_breakout.h"

ME::GameBreakout::GameBreakout() : Game() {}

ME::GameBreakout::~GameBreakout() {}

void ME::GameBreakout::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneBreakout();
    scene->Init();
    brkScene = dynamic_cast<ME::SceneBreakout*>(scene);
    ballTransform = brkScene->instancedSpriteTransforms[brkScene->ballIndex];
    ballInstanceData = brkScene->spriteInstanceData[brkScene->ballIndex];
    ballVelocity = brkScene->ballVelocity;

    // Initialize the physics scene for Breakout.
    physicsScene = new ME::PhysicsSceneBreakout();
    physicsScene->Init(scene);
    physicsSystem->SetScene(physicsScene);

    ballCollider = &(physicsScene->dynamicColliders[0]);

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
    ballCollider->UpdateTransform(*ballTransform);
}

void ME::GameBreakout::End() {
    Game::End();
    delete scene;
    delete physicsScene;
}
