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
    ballCollider->UpdateTransform(*ballTransform, brkScene->ballCollScaleMult);
    brkScene->instancedSpriteRenderers[brkScene->ballIndex]->bDirty = true;
}

void ME::GameBreakout::Update(double deltaTime) {
    Game::Update(deltaTime);

    TranslateBall(
        Vec2(ballVelocity.x * deltaTime * brkScene->ballSpeed, ballVelocity.y * deltaTime * brkScene->ballSpeed));
}

void ME::GameBreakout::End() {
    delete scene;
    delete physicsScene;
    Game::End();
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
        brkScene->instancedSpriteRenderers[b->GetID()]->atlasIndex = 0;  // Make it black (invisible).
        brkScene->instancedSpriteRenderers[b->GetID()]->color = ME::Color::Black();
        brkScene->instancedSpriteRenderers[b->GetID()]->bDirty = true;
        ++score;
    }

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score:%03u", score);
    brkScene->textRenderers[1]->SetText(scoreText);
    ME::LogDebug("Score: " + std::to_string(score));

    delete result;
}

bool ME::GameBreakout::IsDestructible(uint32_t index) const {
    if (index < brkScene->gridCount) {
        return brkScene->instancedSpriteRenderers[index]->atlasIndex != 0;
    }

    return false;
}
