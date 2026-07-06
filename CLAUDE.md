# The Village Shop

C++20 multiplayer village-shop simulation game. Player runs a shop in a village where
every villager is individually simulated with needs (food, shelter, luxury goods); AI
competitors open rival shops; the village population grows or shrinks based on how well
its needs are met; a multiplayer mode lets players compete in a shared village simulation.

**Current actual state**: early engine-building phase. Renderer backends (DX12/Metal),
networking, physics, audio, and animation systems are being built out. The real shop/economy
gameplay (`VillageGame`, see [client/CLAUDE.md](client/CLAUDE.md)) exists, but many `game_*`/
`scene_*` files in `client/src/` are tech-demo scenes (breakout, dice, falling sand, game of
life, RPG test) used to exercise engine subsystems, not shipped features. `client/doc/tdd_v1.txt`
and `server/doc/tdd_v1.txt` are both empty placeholders — no formal TDD content yet.

Gameplay economy rules that do exist are documented in `design/v1/design_v1.md` (short, ~34
lines): a day-based simulation (1 round = 1 day) with cost/price/demand/supply, village
population/growth, bank loans/interest, and per-shop cash/stock/preference/discount/pnl.

## Folder map
- `shared/` — code shared by client and server: networking protocol, math, physics, RNG,
  data structures, file I/O parsers. See [shared/CLAUDE.md](shared/CLAUDE.md).
- `client/` — game client (rendering, scene, game logic, UI, audio, input, world).
  See [client/CLAUDE.md](client/CLAUDE.md).
- `server/` — game server (UDP networking, main loop). See [server/CLAUDE.md](server/CLAUDE.md).
- `client_package/` — separate CLI tool that cooks/packages `client/resources` for
  distribution (HLSL→CSO via dxc.exe, textures→DDS via texconv.exe). Not another client.
- `client_tests/` — unit tests using a hand-rolled `TEST`/`EXPECT_EQ` macro framework (no
  GoogleTest). Covers only `Grid`, `RingBuffer`, ini_parser, and Vec3/Vec4 math — networking,
  physics, fixed-point math, and the RNG are untested. **Currently commented out** of the
  root build (`add_subdirectory(client_tests)` disabled in root `CMakeLists.txt`).
- `logging/` — minimal header-style logging library (colored console output, no file sinks,
  no severity filtering).
- `automation/` — local dev-helper shell scripts, not CI (no GitHub Actions in the repo):
  clang-format-all, mac codesigning, LOC counts, and a trivial "launch client+server, check
  exit codes" smoke test. Not a real integration test suite.
- `design/` — game design docs (currently just `design/v1/design_v1.md`).

## Build system
- Root `CMakeLists.txt`: C++20, builds `logging` → `shared` → `client` → `server` →
  `client_package` in order (`client_tests` disabled).
- **RTTI and exceptions are disabled globally** (MSVC: `/GR- /EHsc` + `_HAS_EXCEPTIONS=0`;
  else: `-fno-rtti -fno-exceptions`). Do not write or suggest code using `try/catch`,
  `dynamic_cast`, or `typeid`.
- Platform matrix maintained in parallel: Windows (DX12 renderer, Winsock2 networking,
  `VG_WIN` define) and Mac (Metal renderer, Cocoa, `VG_MAC` define), plus an ASCII/CLI
  renderer + headless input backend for a console-only build.

## Dependencies
README says "no external dependencies," but this isn't strictly true — vendored code exists:
- `shared/third_party/json` — cJSON, used for all JSON parsing (texture atlases, animation
  clips, wave data).
- `client/third_party/miniaudio-0.11.23` — audio playback backend.
- `client/third_party/metal` — Metal helper headers for the Mac renderer.

These are vendor code; treat them as opaque dependencies, not project code to modify.

## Dev log
`FPSMilestones.txt` — informal engine-loop perf benchmarks from March 2025 (basic loop,
logging overhead, fixed-frame-rate cap, RNG generation). Not gameplay milestones.
