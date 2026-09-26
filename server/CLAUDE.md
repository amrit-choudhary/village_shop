# server/

Game server. See root [CLAUDE.md](../CLAUDE.md) for project-level context, and
[shared/CLAUDE.md](../shared/CLAUDE.md) for the wire protocol shared with the client.

## Main loop
`src/main.cpp` — minimal loop: constructs `ME::Time::TimeManager` fixed at `FPS_60` and a
`ME::SocketServer`; loops on a global atomic `ServerRunning`
(`src/misc/global_vars.h`/`.cpp`), calling `socketServer.Update(deltaTime)` only on ticks
that `timeManager.Update()` says should fire (fixed-frame-rate gating).

## Networking (`src/net/`)
- `ME::SocketServer` (`socket_server.h/.cpp`) — platform-independent logic: packet dispatch,
  client list, chat/data relay.
- `ME::PlatformSocketServer` — virtual base with empty default Init/Update/End/SendPacket.
- `SocketServerWin` (`socket_server_win.*`, `VG_WIN`) / `SocketServerMac`
  (`socket_server_mac.*`, `VG_MAC` **and** `VG_LINUX`) — platform implementations. Windows uses
  raw Winsock2: non-blocking UDP (`socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)`, `ioctlsocket`),
  bound to **port 9310** (from `resources/config/settings.ini`, default 9310), `recvfrom`/`sendto`.
  `SocketServerMac` is plain POSIX sockets (`fcntl` O_NONBLOCK), so it serves Linux unchanged;
  the name is historical. It mirrors the Windows file's structure line for line.
- Verified working: Windows server, Mac server, and Linux (Debian 13, g++ 14) server on a cloud
  VM with Windows clients connecting over LAN/internet. Cloud hosting needs UDP 9310 opened in
  the provider firewall.
- `SocketServer::ProcessPacket` reads version/verb/clientID then dispatches by `Verb`:
  - `CONNECT` → assigns a clientID, `SendConnected`
  - `PING` → `SendPong`
  - `CHAT_SEND` → `HandleChat`, broadcasts to all other clients
  - `DATA_SEND` → `HandleData`, relays 3 `FP_24_8` values to other clients
  - `ACK`, `AUTH`, `DISCONNECT` verbs exist in the enum but are **unhandled stubs** — no
    authentication and no reliability/ack layer exists yet.
- Known issues: `PING`/chat/data/score handlers index `connectedClients[clientID]` with the
  client-supplied ID and no range check (unknown ID reads out of bounds); replies send the whole
  fixed-size `PacketSmall` buffer (trailing zeros), not just the bytes written; packet reads
  have no bounds checking. Don't expose the server publicly for long.

## Database (`src/db/`)
POC persistence: `ME::ScoreDB` (`score_db.h/.cpp`) keeps one global high score in an embedded
SQLite file, `village_shop.db`, created next to the server executable
(`ME::Utils::GetExecutableDirPath()`). Table `highscore` is a single row (`id` pinned to 1).
`Open` loads the value into memory; `SubmitScore` writes only when the score beats it.
- `SocketServer` owns the `ScoreDB`: opened in `Init`, closed in `End` (`main.cpp` never calls
  `End()`, harmless since each write is committed). `HandleScore` calls `SubmitScore`; a new high
  score is broadcast to all clients as `HIGHSCORE_RECV` (0x66, one `uint32`), and a new client
  gets the current value right after `CONNECTED`.
- SQLite is the vendored amalgamation in `third_party/sqlite3/` (opaque, don't modify), built as
  its own `sqlite3` static library by that folder's `CMakeLists.txt`. Compile flags are a minimal
  feature set: `SQLITE_THREADSAFE=0` (server is single-threaded), no load-extension, no UTF-16, no
  deprecated/trace/progress/decltype/get_table APIs, `SQLITE_DQS=0`. Consequences for new code:
  use the UTF-8 APIs only (`sqlite3_open`, `sqlite3_prepare_v2`), single-quote SQL string
  literals, and call SQLite from the main thread only. Warnings are silenced for that target.
- Calls are synchronous inside the 60 Hz tick; fine for occasional writes, batch or thread later.
- No per-player data: `clientID` is connection order, and `AUTH` is still a stub.
- `sqlite3.c` is a slow compile (minutes at `-O3` on a small VM, uses ~1 GB RAM); it is built once
  and only rebuilt if its file, flags, or the build type change.

## Docs
`server/doc/tdd_v1.txt` is empty (0 bytes) — no server design doc exists; behavior must be
read from code.

## Build
`server/CMakeLists.txt` globs all `.cpp` under `server/src`, builds `VillageShop_Server`,
links `VillageShop_Shared` and `sqlite3` (from `third_party/sqlite3`), defines `VG_WIN`/`VG_LINUX`/`VG_MAC` per platform, links
`Ws2_32` on Windows.

The resources folder (`resources/config/settings.ini`) is not copied by CMake; it is looked up
next to the executable (`Utils::SetPaths` strips the exe name, a hardcoded 19 chars on
Mac/Linux, so run it as `./VillageShop_Server` or by full path, never the bare name). If the
ini is missing the server falls back to port 9310.

Linux/Mac build of just the server: `cmake -B build && cmake --build build --target
VillageShop_Server` (use `-j1` on low-RAM machines). Headers must include `<cstdint>` directly:
MSVC/Apple clang hide missing ones, g++ does not.
