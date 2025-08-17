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

void ME::GameBreakout::TranslateBall(Vec2 delta) {
    Vec3 newBallPosition = ballTransform->GetPosition() + Vec3(delta.x, delta.y, 0.0f);
    ballTransform->SetPosition(newBallPosition);

    ballInstanceData->modelMatrixData = ballTransform->GetModelMatrix().GetData();

    ballCollider->UpdateTransform(*ballTransform, brkScene->ballCollScaleMult);
}

void ME::GameBreakout::Update(double deltaTime) {
    Game::Update(deltaTime);

    TranslateBall(
        Vec2(ballVelocity.x * deltaTime * brkScene->ballSpeed, ballVelocity.y * deltaTime * brkScene->ballSpeed));
}

void ME::GameBreakout::End() {
    Game::End();
    delete scene;
    delete physicsScene;
}

void ME::GameBreakout::CollisionCallback(ColliderAABB *a, ColliderAABB *b, CollisionResultAABB *result) {
    // By convention, the ball is always the first collider.
    TranslateBall(result->seperation);

    if (result->normal == Vec2::Up || result->normal == Vec2::Down) {
        ballVelocity.y = -ballVelocity.y;
    }
    if (result->normal == Vec2::Left || result->normal == Vec2::Right) {
        ballVelocity.x = -ballVelocity.x;
    }

    if (IsDestructible(b->GetID())) {
        b->isEnabled = false;
        brkScene->spriteInstanceData[b->GetID()]->atlasIndex = 0;  // Set to black if hit.
        ++score;
    }

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score:%03u", score);
    brkScene->textRenderers[1]->SetText(scoreText);
    brkScene->UpdateTextInstanceData();
    ME::LogDebug("Score: " + std::to_string(score));

    delete result;
}

bool ME::GameBreakout::IsDestructible(uint32_t index) const {
    if (index < brkScene->gridCount) {
        return brkScene->spriteInstanceData[index]->atlasIndex != 0;
    }

    return false;
}
