#include <atomic>
#include <iostream>
#include <thread>

#include "FileIO/INI/INIParser.h"
#include "Game/game.h"
#include "Input/InputManager.h"
#include "Rendering/renderer.h"
#include "TimeManagement/TimeManager.h"
#include "misc/global_vars.h"
#include "src/logging.h"

int main(int argc, char **argv) {
  // Read game params from file.
  INIMap iniMap = Load();
  int fps = std::atoi(iniMap["settings"]["fps"].c_str());
  int maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

  // Init global variables.
  VG::Time::TimeManager timeManager;
  timeManager.Init(fps);
  bool shouldTick = false;
  double deltaTime = 0.0f;

  InputManager inputManager;
  inputManager.Init();
  Game game;
  game.Init(&timeManager);
  Renderer renderer;
  renderer.Init();

  // Game Loop.
  while (GameRunning) {
    shouldTick = timeManager.Update();

    // Game Tick.
    if (shouldTick) {
      deltaTime = timeManager.GetDeltaTime();

      inputManager.Update(deltaTime);
      game.Update(deltaTime);
      // renderer.Update(game.GetBuffer());
    }

    if (timeManager.GetTimeSinceStartup() > maxRunTime) {
      GameRunning = false;
    }
  }

  // Game End.
  timeManager.End();
  inputManager.End();
  game.End();
  renderer.End();
}
