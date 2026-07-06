# client/

Game client. See root [CLAUDE.md](../CLAUDE.md) for project-level context.

## Entry points / boot flow
- `src/main/main_win.cpp` (`ME::GameMain`, DX12) — reads `fps`/`maxRunTime` from an INI,
  then initializes systems in order: InputManager → Connection → PhysicsSystem →
  AudioSystem → AnimationSystem → `Game::Init/Start` → `Renderer::InitDX` + `SetScenes`.
  Fixed-tick loop via shared `TimeManager`: Input pre/update/post → `game.Update` →
  `renderer.Update/Draw` → connection/physics/animation/audio `.Update`.
- `src/main/main_mac.cpp` / `main_mac2.cpp` — Mac/Metal/Cocoa mirror of the above.
- `src/main/main_cli.cpp` — headless console variant (ASCII renderer, CLI input manager).
- `src/platform/` — platform glue: mac `NSApplication`/Metal (`app_delegate`, `metal_view`,
  `view_delegate`), win `WinMain`/window proc (`platform/win/win_main.cpp`).

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
  `PlatformInputManager` (win/mac/cli); `net/connection.h` wraps `PlatformConnection` (win/mac).

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
  feeds positions into `SceneUI`'s sprite/text transform arrays. `ui/geometry.*` has the
  rect/bounds math it uses.

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
