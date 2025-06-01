/**
 * Breakout game scene.
 */

#pragma once

#include "scene.h"

namespace ME {

class SceneBreakout : public Scene {
   public:
    SceneBreakout();
    virtual ~SceneBreakout() override;

   public:
    const uint8_t gridX = 30;
    const uint8_t gridY = 42;
    const uint8_t gridXby2 = gridX / 2;
    const uint8_t gridYby2 = gridY / 2;
    const uint32_t gridCount = gridX * gridY;
    const uint8_t brickWidth = 40;
    const uint8_t brickHeight = 40;
    const uint8_t brickPadding = 0;
    const int16_t originX = -600;
    const int16_t originY = -900;
    const uint8_t ballSize = 50;
    const uint8_t ballSpeed = 5;
    const uint16_t ballIndex = gridCount;  // Index for the ball in instancedSpriteTransforms.
    const float ballInitX = 0.0f;
    const float ballInitY = -600.0f;
    const uint16_t paddleIndex = gridCount + 1;  // Index for the paddle in instancedSpriteTransforms.
    const uint16_t paddleSizeX = 250;
    const uint16_t paddleSizeY = 40;
    const float paddleInitX = 0.0f;
    const float paddleInitY = -900.0f;

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
    ME::Color colorPalette[8] = {ME::Color(0.98f, 1.0f, 1.0f, 1.0f),   ME::Color(0.23f, 0.35f, 0.46f, 1.0f),
                                 ME::Color(0.84f, 0.26f, 0.22f, 1.0f), ME::Color(0.40f, 0.54f, 0.59f, 1.0f),
                                 ME::Color(0.92f, 0.58f, 0.33f, 1.0f), ME::Color(0.65f, 0.81f, 0.28f, 1.0f),
                                 ME::Color(0.88f, 0.89f, 0.88f, 1.0f), ME::Color(0.67f, 0.80f, 0.82f, 1.0f)};

    void CreatePaddle();
    void CreateBall();
};

}  // namespace ME
