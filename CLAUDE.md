@../../moksha/moksha/CLAUDE.md

# The Village Shop

A multiplayer village-shop simulation built on the Moksha engine. The player runs a shop in a village where
every villager is simulated with needs (food, shelter, luxury goods); AI competitors open rival shops; the
population grows or shrinks with how well its needs are met; in multiplayer, players compete in one shared
village. Economy rules: [design/v1/design_v1.md](design/v1/design_v1.md) (day-based rounds,
cost/price/demand/supply, population growth, bank loans, per-shop cash/stock/discount/pnl).

**Current state:** a fresh start on the engine. The client opens a window with an empty village scene and a
title bar; the server accepts connections and has no village messages yet.

## Layout
- `client/` (`VillageShop_Client`, `moksha_add_game`): `game_main.cpp` (`MOKSHA_GAME`), `game/village_game.*`
  (`VS::VillageGame`), `scene/scene_village.*` (the world), `scene/scene_ui_village.*` (HUD), `resources/`
  (`res_list.json`, `config/settings.ini`, the game's own assets; engine shaders, font and UI atlas come
  from the engine).
- `server/` (`VillageShop_Server`, `moksha_add_server`): `main.cpp` + `VS::VillageServer`
  (`IGameServerListener`), `resources/config/settings.ini` (`[settings] port`).
- Add `shared/` (a `VillageShop_Shared` library) once client and server share code, e.g. the village's
  message verbs (from `GameProtocol::FIRST_GAME_VERB`). Pass it to both through `LIBRARIES`.
- Game code is in `namespace VS`. Includes are qualified from this repo's root (`"client/game/village_game.h"`);
  engine headers as `"moksha/..."`.
- `design/` (game design), `automation/` (old helper scripts, written for the previous layout),
  `screenshots/` (earlier prototype), `FPSMilestones.txt` (engine-loop benchmarks from March 2025).

## Engine link
- Engine pin: `MOKSHA_PIN` in the root `CMakeLists.txt`. Presets: `pinned` (`build/`) and `live`
  (`build-live/`, from the gitignored `CMakeUserPresets.json`, building `../../moksha/moksha`).
- Engine changes the game needs go into the engine (generic, no village specifics), pushed first; then bump
  the pin. Engine conventions (imported above) apply to this repo too.

## Pi deployment (me_server_manager)
The Pi runs servers through a separate repo, **me_server_manager** (PC: `D:\Dev\me_server_manager\me_server_manager`,
Pi: `/home/amrit/gamedev/me_server_manager/me_server_manager`): a Python web dashboard (port 8080, systemd
service) that builds, starts, stops and monitors servers. Its `config.json` still has this repo's previous
targets and steps; it needs updating before the dashboard can run the new server:
- **Build**, in the Pi clone: `git pull --ff-only`, `cmake --preset pinned -DMOKSHA_BUILD_CLIENT=OFF
  -DCMAKE_BUILD_TYPE=Release`, `cmake --build --preset pinned --target VillageShop_Server -j2`. The build
  copies `resources/` next to the binary; no manual copy steps.
- **Binary**: `build/server/VillageShop_Server`. There is no content server in this repo now.
- **Runtime contract**: started with cwd = the binary's folder, stdout/stderr captured to a log file. Stopped
  with SIGTERM, then SIGKILL after 5 s (the engine has no SIGTERM handler yet). Any exit counts as a crash and
  triggers a restart.
