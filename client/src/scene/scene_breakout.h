/**
 * Breakout game scene.
 */

#pragma once

#include "../../../shared/src/math/vec2.h"
#include "scene.h"

namespace ME {

class SceneBreakout : public Scene {
   public:
    SceneBreakout();
    virtual ~SceneBreakout() override;

   public:
    const uint16_t gridX = 32;
    const uint16_t gridY = 42;
    const uint16_t gridXby2 = gridX / 2;
    const uint16_t gridYby2 = gridY / 2;
    const uint32_t gridCount = gridX * gridY;
    const uint16_t brickWidth = 40;
    const uint16_t brickWidthby2 = brickWidth / 2;
    const uint16_t brickHeight = 40;
    const uint16_t brickHeightby2 = brickHeight / 2;
    const uint16_t brickPadding = 0;
    const int16_t originX = -640;
    const int16_t originY = -900;
    const uint16_t ballSize = 50;
    const uint16_t ballSpeed = 600;
    const Vec2 ballVelocity{1.0f, 1.0f};       // Initial velocity of the ball.
    const uint16_t ballIndex = gridCount + 4;  // Index for the ball in instancedSpriteTransforms.
    const float ballInitX = 0.0f;
    const float ballInitY = -600.0f;
    const float ballCollScaleMult = 0.80f;       // Scale multiplier for the ball collider.
    const uint16_t paddleIndex = gridCount + 5;  // Index for the paddle in instancedSpriteTransforms.
    const uint16_t paddleSizeX = 250;
    const uint16_t paddleSizeY = 40;
    const float paddleInitX = 0.0f;
    const float paddleInitY = -880.0f;
    const uint16_t wallIndexBottom = gridCount + 0;  // Bottom wall index.
    const uint16_t wallIndexRight = gridCount + 1;   // Right wall index.
    const uint16_t wallIndexTop = gridCount + 2;     // Top wall index.
    const uint16_t wallIndexLeft = gridCount + 3;    // Left wall index.
    const uint16_t wallHeight = 30;
    const uint16_t wallHeightby2 = wallHeight / 2;
    const uint16_t wallSizeX = (gridX * brickWidth) + (2 * wallHeight);   // Width of the walls.
    const uint16_t wallSizeY = (gridY * brickHeight) + (2 * wallHeight);  // Height of the walls.

    virtual void Init() override;
    virtual void CreateResources() override;
    virtual void BuildLights() override;
    virtual void BuildCamera() override;
    virtual void BuildTransforms() override;
    virtual void BuildMeshRenderers() override;
    virtual void BuildSpriteTransforms() override;
    virtual void BuildSpriteRenderers() override;
    virtual void BuildInstancedSpriteTransforms() override;
    virtual void BuildInstancedSpriteRenderers() override;
    virtual void BuildTextRenderers() override;

   private:
    ME::Color colorPalette[8] = {ME::Color("#fbffff"), ME::Color("#3a5975"), ME::Color("#d74339"),
                                 ME::Color("#678b97"), ME::Color("#eb9454"), ME::Color("#a7ce47"),
                                 ME::Color("#e1e3e1"), ME::Color("#accbd2")};

    void CreateWalls();
    void CreatePaddle();
    void CreateBall();
};

}  // namespace ME
