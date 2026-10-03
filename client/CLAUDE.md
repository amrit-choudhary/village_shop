# client/

Game client. See root [CLAUDE.md](../CLAUDE.md) for project-level context.

## Entry points / boot flow
- `src/main/main_win.cpp` (`ME::GameMain`, DX12) — reads `fps`/`vsync` from an INI, then
  initializes systems in order: InputManager → GameClient → PhysicsSystem → AudioSystem →
  AnimationSystem → `Game::Init/Start` → `Renderer::InitDX` + `SetScenes`. Loop via shared
  `TimeManager::BeginFrame()`: Input pre/update → 0..N `game.FixedUpdate`/physics/animation
  steps at a constant simulation dt (`TimeConfig::fixedStepFPS`) → `game.Update`/UI/debug at
  the real, variable per-frame dt → `renderer.Update/Draw` → gameClient/contentClient/audio `.Update`. Vsync
  is on by default (`RendererDX::SetVsyncEnabled`), which paces presentation to the display's
  refresh rate; `TimeConfig::frameRateCapFPS` (sleep-based) only takes over when vsync is off.
- `src/main/main_mac.cpp` (`ME::GameMain`, Metal) — **currently does not compile**: still uses the
  pre-revamp `TimeManager` API (`Init(fps, false)`, `Update()`, `GetDeltaTime()`), lacks the fixed-step loop,
  animation/audio/debug systems and `ContentClient`; to be fixed in one pass later. Otherwise same shape as Windows: `Init(MTL::Device*, MTK::View*)`
  receives the Metal handles once (mirrors `Init(HWND)`), wires `RendererMetal::InitMTL` +
  `SetScene`, and `Update()` ends with `renderer.Update(); renderer.Draw();`. Input arrives via
  `HandleKeyEvent`/`HandleMouseMove`/`HandleMouseButton`, forwarded synchronously from
  `platform/mac/metal_view.mm`'s NSEvent handlers — mirrors `WindowProcW` → `GameMain::HandleInput`.
  As of the last pass this was fixed after being disconnected mid-refactor (renderer commented
  out, dead `SetViewAndDevice`, input routed through a stdin-reading `InputManagerCLIMac` instead
  of a real `InputManagerMac`) — see git history around commit `b020075` for context if similar
  drift happens again.
- `src/main/main_cli.cpp` — headless console variant (ASCII renderer, CLI input manager). Note:
  gated by `VG_CLI`, which is **not currently defined anywhere in CMakeLists** — this file
  currently compiles to nothing; the headless build isn't actually wired up yet.
- `src/platform/` — platform glue: mac `NSApplication`/Metal (`app_delegate`, `metal_view`,
  `view_delegate`), win `WinMain`/window proc (`platform/win/win_main.cpp`). On mac, each `.mm`
  file is a thin forwarder into `GameMain`/engine classes, deliberately mirroring the Windows
  shape: `app_delegate.mm` ≈ `win_main.cpp`'s window setup, `view_delegate.mm`'s `drawInMTKView:`
  ≈ the Windows message loop's `game.Update()` call, `metal_view.mm`'s event handlers ≈ `WindowProcW`.

## Architecture (not ECS)
Data-oriented "Scene as struct-of-arrays," not an entity-component system:
- `scene/scene.h` (`Scene`) and `scene/scene_ui.h` (`SceneUI`) are plain data containers —
  parallel arrays of transforms/renderers/colliders/lights/cameras/sprite data, **no
  rendering-API calls**. Per-game subclasses (`scene_rpg`, `scene_breakout`, etc.) populate
  them via `Build*` virtuals.
- `game/game.h` (`Game`) owns a `Scene`+`SceneUI`, drives gameplay each tick, and exposes
  `CollisionCallback` for physics to report back into. Game subclasses populate the scene.
- `rendering/` consumes `Scene`/`SceneUI` each frame: `Renderer` (shared facade) picks a
  `PlatformRenderer` — DX12 (`rendering/directx/`), Metal (`rendering/metal/`), or ASCII
  (`rendering/ascii/renderer_ascii.*`, used by the CLI build) — and pulls dirty scene data
  to build/update GPU resources. Clean separation: simulation-facing data vs backend draw code.
- Same platform-strategy pattern repeats elsewhere: `audio/audio_system.h` wraps `IAudioImpl`
  (miniaudio vs `audio_impl_dummy.h` no-op); `input/input_manager.h` wraps
  `PlatformInputManager` (win/mac/cli). Exception: networking has no per-platform classes; per-OS socket
  code lives only in `shared/src/net/socket_platform_*`.

## The actual game vs tech demos
- `game/village_game.h/.cpp` (`VillageGame : Game`) is the real product: a `Shop` struct
  (cash, stock, preference, discount, pnl, loan, interest) with day-cycle simulation
  (`DayChange`, `BuyStock`, `RefreshDisplay`, buy/sell price averaging via `Random`).
- `game/villager.h/.cpp` — small per-villager data classes (`VHealth`, `VHunger`, `VGold`),
  not a full ECS component system.
- Other `game_*` / `scene_*` files (`game_breakout`, `game_dice_simple`, `game_falling_sand`,
  `game_game_of_life`, `game_character_test`, `game_rpg` and their matching scenes) are
  engine tech-demo scenes used to exercise physics/animation/rendering — not shipped features.
  Don't assume they're part of the real game when reading or extending them.
- `world/tile.h/.cpp` — `TileData`/`Tile` structs exist; `world/tilemap.h/.cpp` (`TileMap`)
  is still an **empty stub** — the tile-based village map is not implemented yet.
- `ui/ui_layout_engine.h/.cpp` (`UILayoutEngine`) — small retained-mode layout system that
  feeds positions into `SceneUI`'s sprite/text transform arrays. `ui/ui_rect.*` has the
  `UIRect`/`UIAnchor` rect/bounds math it uses.

## Game server client (`src/net/game_client.*`)
`GameClient` (owned by `GameMain`, passed to games via `Game::SetGameClientRef`) is the UDP client for the game server: one `Net::UdpSocket` on an
OS-chosen port, server address from `serverIP` / `serverPort` in `settings.ini` (defaults `127.0.0.1:9310`).
`Update` drains received datagrams each frame and turns them into delegates (`onConnected`, `onPong`,
`onScoreReceived`, `onHighScoreReceived`); chat and game data are only logged. Send functions do nothing until
`CONNECTED` assigned a clientID (except `SendConnectRequest`). Wire format: `shared/src/net/game_protocol.h`.
Networking itself is started once in `GameMain::Init` (`Net::InitNetworking`), not by `GameClient`.

## Content sync (`src/net/content_client.*`)
`ContentClient` (owned by `GameMain`, Windows only for now) syncs `dlc/` (next to the exe) with the content
server at startup, in the background, one state-machine step per frame: connect (5 s timeout) →
`GET_MANIFEST` → `GET_FILE` for each new/changed/missing file → delete files the server dropped → done.
Files are written straight into `dlc/`; `dlc/manifest.json` always describes what is on disk (a file's version
is recorded only after it is fully written, so an interrupted sync resumes). On failure the game carries on
with existing files. Server address: `contentServerIP` / `contentServerPort` in `resources/config/settings.ini`
(defaults `127.0.0.1:9311`). No game code reads from `FileRoot::Dlc` yet; a loading-screen gate that waits for
the sync before gameplay is planned. 1 MB receive buffer is heap-allocated (`GameMain` lives on the stack).
`resources/` is copied to the build folder only by the manual `copy_resources` target.

## Data-driven resources
`utils/json_utils.h/.cpp` parses JSON (via vendored cJSON) into `TextureAtlasProperties`,
`SpriteAnimClip`, and `WaveData`/`SingleWave` — texture atlases, sprite animations, and wave
config are all data-driven from `client/resources/`.

## Related tool
`client_package/` (sibling to `client/`, not inside it) is a separate CLI tool that cooks and
packages `client/resources` into shippable form (HLSL→CSO, textures→DDS). Windows-only so far.

## Out of scope
`client/third_party/` (miniaudio, Metal helper headers) is vendored code — treat as opaque,
don't deep-dive into its internals.
