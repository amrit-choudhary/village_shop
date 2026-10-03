#include "game_net_test.h"

#include <cstdio>

#include "client/src/debug/debug_system.h"
#include "client/src/ui/button.h"

ME::GameNetTest::GameNetTest() : Game() {}

ME::GameNetTest::~GameNetTest() {}

void ME::GameNetTest::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);

    // Plain, un-subclassed Scene: no Build*() overrides, so no meshes/sprites/lights - renders
    // as a black background.
    scene = new ME::Scene();
    scene->Init();

    sceneUINetTest = new ME::SceneUINetTest();
    sceneUINetTest->Init();
    uiScene = sceneUINetTest;

    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnScoreButtonClicked>(sceneUINetTest->GetScoreButton()->onClick,
                                                                        this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnConnected>(gameClient->onConnected, this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnPong>(gameClient->onPong, this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnScoreReceived>(gameClient->onScoreReceived, this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnHighScoreReceived>(gameClient->onHighScoreReceived, this);

    gameClient->SendConnectRequest();

    ME::Log("Net Test Game Start!");
}

void ME::GameNetTest::Start() {
    Game::Start();
}

void ME::GameNetTest::Update(double deltaTime) {
    Game::Update(deltaTime);

    pingTimer += deltaTime;
    if (pingTimer >= kPingIntervalSeconds) {
        pingTimer -= kPingIntervalSeconds;
        gameClient->SendPing();
        ++pingsSent;
        RefreshPingLabel();
    }
}

void ME::GameNetTest::OnConnected() {
    char buf[32];
    snprintf(buf, sizeof(buf), "Connected as Client %d", gameClient->GetClientID());
    sceneUINetTest->GetStatusLabel()->SetText(buf);
}

void ME::GameNetTest::OnPong() {
    ++pongsReceived;
    RefreshPingLabel();
}

void ME::GameNetTest::OnScoreReceived() {
    char buf[32];
    snprintf(buf, sizeof(buf), "Opponent Score: %u", gameClient->GetLastScore());
    sceneUINetTest->GetOpponentScoreLabel()->SetText(buf);
}

void ME::GameNetTest::OnHighScoreReceived() {
    char buf[32];
    snprintf(buf, sizeof(buf), "Global High: %u", gameClient->GetHighScore());
    sceneUINetTest->GetHighScoreLabel()->SetText(buf);
}

void ME::GameNetTest::OnScoreButtonClicked() {
    ++score;

    char buf[32];
    snprintf(buf, sizeof(buf), "Your Score: %u", score);
    sceneUINetTest->GetYourScoreLabel()->SetText(buf);

    gameClient->SendScore(score);
}

void ME::GameNetTest::RefreshPingLabel() {
    char buf[48];
    snprintf(buf, sizeof(buf), "Pings: %u / Pongs: %u", pingsSent, pongsReceived);
    sceneUINetTest->GetPingLabel()->SetText(buf);
}

void ME::GameNetTest::End() {
    // sceneUINetTest and uiScene alias the same object - deleting uiScene destroys the widgets
    // SceneUINetTest owns (see SceneUINetTest::~SceneUINetTest()).
    delete uiScene;
    delete scene;

    Game::End();
    ME::Log("Net Test Game End!");
}
