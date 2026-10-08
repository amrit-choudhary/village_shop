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
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnQuizQuestion>(gameClient->onQuizQuestion, this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnAnswer0Clicked>(sceneUINetTest->GetAnswerButton(0)->onClick, this);
    ME::Delegate::Bind<GameNetTest, &GameNetTest::OnAnswer1Clicked>(sceneUINetTest->GetAnswerButton(1)->onClick, this);

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

void ME::GameNetTest::OnQuizQuestion() {
    const ME::Net::GameProtocol::QuizQuestion& question = gameClient->GetLastQuizQuestion();

    snprintf(questionText, sizeof(questionText), "Q%u: %u %c %u = ?", question.id, static_cast<unsigned>(question.lhs),
             question.op, static_cast<unsigned>(question.rhs));
    sceneUINetTest->GetQuizQuestionLabel()->SetText(questionText);

    char buf[8];
    for (int i = 0; i < ME::SceneUINetTest::ANSWER_COUNT; ++i) {
        snprintf(buf, sizeof(buf), "%u", static_cast<unsigned>(question.options[i]));
        sceneUINetTest->GetAnswerLabel(i)->SetText(buf);
    }
    SetAnswerButtonsVisible(true);

    if (timerManager == nullptr) {
        return;
    }
    // A question arriving early replaces the open one, so its timeout must not close the new one.
    timerManager->Clear(answerTimer);
    answerTimer = timerManager->SetTimeout(ME::Delegate::Make<GameNetTest, &GameNetTest::OnAnswerTimeout>(this),
                                           kAnswerWindowSeconds);
}

void ME::GameNetTest::OnAnswerTimeout() {
    SetAnswerButtonsVisible(false);

    char buf[64];
    snprintf(buf, sizeof(buf), "%s  Time up!", questionText);
    sceneUINetTest->GetQuizQuestionLabel()->SetText(buf);
    ME::Log("Quiz: time up");
}

void ME::GameNetTest::OnAnswer0Clicked() {
    PickAnswer(0);
}

void ME::GameNetTest::OnAnswer1Clicked() {
    PickAnswer(1);
}

void ME::GameNetTest::PickAnswer(int index) {
    if (timerManager != nullptr) {
        timerManager->Clear(answerTimer);
    }
    SetAnswerButtonsVisible(false);

    const unsigned picked = gameClient->GetLastQuizQuestion().options[index];
    char buf[64];
    snprintf(buf, sizeof(buf), "%s  Picked %u", questionText, picked);
    sceneUINetTest->GetQuizQuestionLabel()->SetText(buf);
    ME::Log("Quiz: picked ", picked);
}

void ME::GameNetTest::SetAnswerButtonsVisible(bool visible) {
    for (int i = 0; i < ME::SceneUINetTest::ANSWER_COUNT; ++i) {
        sceneUINetTest->GetAnswerButton(i)->SetVisible(visible);
    }
}

void ME::GameNetTest::RefreshPingLabel() {
    char buf[48];
    snprintf(buf, sizeof(buf), "Pings: %u / Pongs: %u", pingsSent, pongsReceived);
    sceneUINetTest->GetPingLabel()->SetText(buf);
}

void ME::GameNetTest::End() {
    // Before the UI goes away: the timeout callback writes to its widgets.
    if (timerManager != nullptr) {
        timerManager->Clear(answerTimer);
    }

    // sceneUINetTest and uiScene alias the same object - deleting uiScene destroys the widgets
    // SceneUINetTest owns (see SceneUINetTest::~SceneUINetTest()).
    delete uiScene;
    delete scene;

    Game::End();
    ME::Log("Net Test Game End!");
}
