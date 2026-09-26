#pragma once

/**
 * Multiplayer networking test/demo game: exercises CONNECT/CONNECTED, PING/PONG, and a
 * SCORE_SEND/SCORE_RECV synced score between exactly two clients, with all state visible in
 * the UI (see SceneUINetTest). Not shipped gameplay - a manual test harness for the network
 * layer, same role as GameUIDemo for the UI subsystem.
 */

#include "game.h"
#include "client/src/scene/scene_ui_net_test.h"

namespace ME {

class GameNetTest : public Game {
   public:
    GameNetTest();
    ~GameNetTest();

    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;
    virtual void Start() override;
    virtual void Update(double deltaTime) override;
    virtual void End() override;

    void OnConnected();
    void OnPong();
    void OnScoreReceived();
    void OnScoreButtonClicked();

   private:
    void RefreshPingLabel();

    // Same object as the base Game::uiScene, kept here with its concrete type so handlers can
    // reach SceneUINetTest's labels without a cast.
    ME::SceneUINetTest* sceneUINetTest = nullptr;

    uint32_t score = 0;
    uint32_t pingsSent = 0;
    uint32_t pongsReceived = 0;
    double pingTimer = 0.0;

    static constexpr double kPingIntervalSeconds = 2.0;
};

}  // namespace ME
