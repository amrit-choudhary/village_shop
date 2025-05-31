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
    const uint8_t gridY = 52;
    const uint8_t gridXby2 = gridX / 2;
    const uint8_t gridYby2 = gridY / 2;
    const uint32_t gridCount = gridX * gridY;
    const uint8_t brickWidth = 32;
    const uint8_t brickHeight = 32;
    const uint8_t brickPadding = 0;
    const int16_t originX = -480;
    const int16_t originY = -800;
    const uint8_t paddleWidth = 100;
    const uint8_t paddleHeight = 20;
    const uint8_t ballSize = 20;
    const uint8_t ballSpeed = 5;

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
    void CreatePaddle();
    void CreateBall();
};

}  // namespace ME
