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
- `content_server/` — separate TCP service that delivers config files (`dlc/`) to clients; the client
  downloads changed files at startup in the background. See [content_server/CLAUDE.md](content_server/CLAUDE.md).
- `client_package/` — separate CLI tool that cooks/packages `client/resources` for
  distribution (HLSL→CSO via dxc.exe, textures→DDS via texconv.exe). Not another client.
- `test_framework/` — hand-rolled unit test framework (no GoogleTest): `TEST(category, name)`
  auto-registers, `EXPECT` records a failure and continues, `ASSERT` records and returns from the
  test, `EXPECT_NEAR` for floats. No exceptions. Each test executable's `main` returns
  `ME::Test::RunAll()` (exit code 0 = all passed).
- `client_tests/` — `VillageShop_Client_Tests`: covers shared code — `Grid`, `RingBuffer`, `Pool`,
  `Span`, `INIParser::Parse`, Vec2/Vec3/Vec4/Vec3i, `FP_24_8`, `Random`/`RandomWt`,
  `ByteWriter`/`ByteReader`, `GameProtocol` helpers, TCP message framing, `ContentManifest`. Links
  only `VillageShop_Shared`; client code itself isn't testable until the client is split into a
  library. Untested: sockets, physics, Matrix4/transform, CSV/DDS parsers, `TimeManager`.
- `server_tests/` — `VillageShop_Server_Tests`: covers `ScoreDB` (SQLite `:memory:`, plus one
  file round-trip). The server has no library target, so the CMake lists the server `.cpp` files
  under test explicitly — add to `SERVER_SOURCES_UNDER_TEST` when testing a new server file.
- `logging/` — minimal header-style logging library (colored console output, no file sinks,
  no severity filtering).
- `automation/` — local dev-helper shell scripts, not CI (no GitHub Actions in the repo):
  clang-format-all, mac codesigning, LOC counts, and a trivial "launch client+server, check
  exit codes" smoke test. Not a real integration test suite.
- `design/` — game design docs (currently just `design/v1/design_v1.md`).

## Build system
- Root `CMakeLists.txt`: C++20, builds `logging` → `shared` → `client` → `server` →
  `content_server` → `client_package`, then `test_framework` → `client_tests` → `server_tests`
  when `VG_BUILD_TESTS` is ON (default; `-DVG_BUILD_TESTS=OFF` skips them).
- **RTTI and exceptions are disabled globally** (MSVC: `/GR- /EHsc` + `_HAS_EXCEPTIONS=0`;
  else: `-fno-rtti -fno-exceptions`). Do not write or suggest code using `try/catch`,
  `dynamic_cast`, or `typeid`.
- Platform matrix maintained in parallel: Windows (DX12 renderer, Winsock2 networking,
  `VG_WIN` define) and Mac (Metal renderer, Cocoa, `VG_MAC` define), plus an ASCII/CLI
  renderer + headless input backend for a console-only build.
- The **server** and **content server** also build and run on Linux (`VG_LINUX`, POSIX sockets); the
  client targets Windows and Mac only. Test host: a Raspberry Pi 4B (Linux ARM64).
- Source files are collected with `file(GLOB ...)`, which runs only at configure time: after adding or
  removing a `.cpp`, re-run `cmake -B build` before building (otherwise: undefined-reference link errors).

## Code conventions
Apply to all project code (not `third_party/`). Existing code that breaks a rule isn't rewritten
unprompted; new code follows them.
- **No exceptions or RTTI** — no `try`/`catch`/`throw`, `dynamic_cast`, `typeid` (see Build system).
  Report failure with return values (`bool`, null pointer, error enum).
- **`class`, never `struct`** — even for plain-data aggregates; use an explicit `public:` label.
- **No `std::vector` in new code** — prefer caller-owned memory (pointer + capacity), fixed-size
  arrays, or `Span<T>`, always with bounds checks. If growth is truly needed, ask first.
- **No singletons / static instance pointers** to give callbacks or systems access to object state.
  Pass explicit bound context instead (the `Delegate` pattern: `void* object` + function pointer, or
  a listener interface). If a global looks like the only option, raise it as a design problem first.
- **Includes are module-qualified from the repo root**: `client/src/...`, `shared/src/...`,
  `server/src/...`, `logging/src/logging.h`, `test_framework/src/...`. No bare `src/...`, no `../`.
- **Comments: max 2 lines**, stating the engine-level contract. Never justify code by what one
  game or demo currently does — this is a general engine many games will be built on.
- **Engine APIs over demo call sites** — if changing an API makes the engine more consistent, change
  it and fix the tech-demo call sites in the same pass; demos carry no backward-compat weight.
- Headers include what they use directly (e.g. `<cstdint>`): MSVC/Apple clang hide missing
  includes, g++ (Linux server builds) does not.
- Format with the repo's `.clang-format` (`automation/clang_format_all_files.sh`).

## Working with AI assistants
- Never `git commit` or stage; the user reviews and commits manually.
- Don't build or run the client/server to verify; the user does that. Ask before running anything.
- For new systems: small, independently buildable steps, simplest first, explain each, then stop.

## Dependencies
README says "no external dependencies," but this isn't strictly true — vendored code exists:
- `shared/third_party/json` — cJSON, used for all JSON parsing (texture atlases, animation
  clips, wave data).
- `client/third_party/miniaudio-0.11.23` — audio playback backend.
- `client/third_party/metal` — Metal helper headers for the Mac renderer.
- `server/third_party/sqlite3` — SQLite amalgamation, server-only; persists the global high score
  (see [server/CLAUDE.md](server/CLAUDE.md)).

These are vendor code; treat them as opaque dependencies, not project code to modify.

## Dev log
`FPSMilestones.txt` — informal engine-loop perf benchmarks from March 2025 (basic loop,
logging overhead, fixed-frame-rate cap, RNG generation). Not gameplay milestones.
