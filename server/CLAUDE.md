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
  (`socket_server_mac.*`, `VG_MAC`) — platform implementations. Windows uses raw Winsock2:
  non-blocking UDP (`socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)`, `ioctlsocket`), bound to
  **port 9310**, `recvfrom`/`sendto`.
- `SocketServer::ProcessPacket` reads version/verb/clientID then dispatches by `Verb`:
  - `CONNECT` → assigns a clientID, `SendConnected`
  - `PING` → `SendPong`
  - `CHAT_SEND` → `HandleChat`, broadcasts to all other clients
  - `DATA_SEND` → `HandleData`, relays 3 `FP_24_8` values to other clients
  - `ACK`, `AUTH`, `DISCONNECT` verbs exist in the enum but are **unhandled stubs** — no
    authentication and no reliability/ack layer exists yet.

## Docs
`server/doc/tdd_v1.txt` is empty (0 bytes) — no server design doc exists; behavior must be
read from code.

## Build
`server/CMakeLists.txt` globs all `.cpp` under `server/src`, builds `VillageShop_Server`,
links `VillageShop_Shared`, defines `VG_WIN`/`VG_LINUX`/`VG_MAC` per platform, links
`Ws2_32` on Windows.
