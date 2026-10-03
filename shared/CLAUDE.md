# shared/

Code shared by client and server. See root [CLAUDE.md](../CLAUDE.md) for project-level context.
Everything here is hand-rolled (no STL containers used for game data, no external libs)
except where noted — this matches the project's "no external dependencies" intent, with
one real exception (cJSON, noted below).

## Networking / wire protocol (`src/net/`)
- **Namespace rule:** everything in `src/net/` lives in `ME::Net` (sockets, framing, protocols). Exception: the
  deprecated `Packet` stays in `ME` until it is deleted. App-level users (`server/` `GameServer`, `client/`
  `GameClient`) stay in `ME`.
- `networking.h` (`Net::InitNetworking`/`ShutdownNetworking`): once per program in each `main`, before any
  socket / after all are closed (WSAStartup/WSACleanup on Windows, no-op on POSIX). Socket users never call it.
- `socket_platform.h` + `socket_platform_win.cpp` / `_posix.cpp` (`Net::SocketPlatform`): the only per-OS
  socket code, TCP and UDP (Winsock vs POSIX incl. SIGPIPE, `SO_EXCLUSIVEADDRUSE`/`SO_REUSEADDR`, non-blocking
  connect via zero-timeout `select` + `SO_ERROR`, UDP `SIO_UDP_CONNRESET` off on Windows, oversized datagrams cut
  to the buffer on every OS). Each .cpp is wrapped in its platform `#ifdef`; both are always compiled.
- UDP stack (game server / client): `net_address.h` (`Net::Address`: IPv4 + port, host byte order) and
  `udp_socket.h` (`Net::UdpSocket`: non-blocking `Open(port)` (0 = OS picks), `SendTo`/`RecvFrom`; `UdpResult`).
- TCP stack (used by `content_server/` and the client's `ContentClient`):
  - `tcp_socket.h` (`Net::TcpSocket`): non-blocking `Listen`/`Accept` (server), `Connect`/`PollConnect`
    (client), `Send`/`Recv`/`Close`; results are `TcpResult` (`WouldBlock` = nothing to do yet, try again).
  - `message_framing.h` (`Net::BeginFrame`/`FinishFrame`, `Net::FrameReceiver`): length-prefix framing over the
    byte stream, caller-owned buffers, rejects impossible lengths.
  - `content_protocol.h` (`Net::ContentProtocol`) and `content_manifest.h` (`Net::ContentManifest`: parse /
    serialize `manifest.json` from memory, safe-path validation, max 256 entries).
- Avoid names that are `windows.h` macros in new APIs (e.g. `GetFreeSpace`, `PeekMessage`, `SendMessage`,
  `DeleteFile`, `MoveFile`, `CreateDirectory`): they get rewritten and break the Windows build.
- `game_protocol.h` (`Net::GameProtocol`) — UDP game wire format: one datagram = u8 version (`VERSION` 0) |
  u8 verb | u8 clientID | payload, `MAX_DATAGRAM_SIZE` 1200. `Verb` ranges: System `0x00-0x1F`, Http
  `0x20-0x3F`, Matchmaking `0x40-0x5F`, Gameplay `0x60-0x7F`. `WriteHeader`/`ReadHeader`, `WriteFP`/`ReadFP`
  (raw 32-bit) over `ByteWriter`/`ByteReader`; `GetVerbName` ("UNKNOWN" for unknown bytes).
- `net_packet.h/.cpp` — `Packet` base wraps a raw `uint8_t*` with a manual read/write cursor
  (`WriteByte/ReadByte/WriteString/ReadString/WriteFP/ReadFP`, direct pointer arithmetic and
  `strcpy`/`reinterpret_cast`, **no bounds checking, no endianness handling**). Fixed-size
  pool subclasses: `PacketSmall`(64) / `PacketMedium`(256) / `PacketBig`(1024) /
  `PacketHuge`(2048) bytes, each `new uint8_t[size]`.
- **`Packet` is deprecated.** `ByteWriter`/`ByteReader` (`src/serialization/byte_writer.h`, `byte_reader.h`:
  bounds-checked, caller-owned memory) is the single binary read/write API for all new code (TCP, UDP, binary files).
  `Packet` stays only until the UDP game client/server migrate to it; don't add new `Packet` uses.
- Server-side consumer: [server/CLAUDE.md](../server/CLAUDE.md).

## Math (`src/math/`)
Custom `Vec2/Vec2i/Vec3/Vec3i/Vec4`, `Matrix4`, `Transform`, `Vec16` (packed/quantized).
`fp_24_8.h` is a hand-rolled 24.8 fixed-point type (int32-backed, ported from the
MikeLankamp/fpm design) used directly in packet I/O (`Packet::WriteFP/ReadFP`) — this signals
**deterministic-simulation intent for multiplayer sync**: anything that must stay in sync
across client/server over the network should go through `FP_24_8`, not raw floats.

## Data structures (`src/datastructure/`)
- `Grid<T>` (`grid.h`) — flat 2D array with 4/8-neighbor queries and a direction enum.
- `RingBuffer<T>` (`ring_buffer.h`) — fixed-capacity circular buffer.
- Both have **deleted copy/move constructors** and manage raw `new[]`/`delete[]` — treat as
  move-unsafe; pass by pointer/reference, don't expect value semantics.

## Physics (`src/physics/`)
`Collider`/`ColliderAABB`, `PhysicsScene` (static/dynamic collider arrays + id-to-index maps,
`Init` takes raw collider arrays/counts, not a `client/` `Scene*`), `PhysicsSystem`
(layer-based collision categorization via `physics_layer.h`, reports collisions through the
`ICollisionListener` interface it defines rather than a concrete `client/` `Game*`). `shared/`
has no `client/` includes as of this writing — keep it that way if touching physics.

## Random (`src/random/`)
`Random` (`random_engine.h/.cpp`) — xoshiro128** PRNG implemented from scratch. `RandomWt` —
weighted-random outcomes via a 10-slot lookup table. `stb_perlin.h/.cpp` — vendored
(public-domain) Perlin noise; treat as opaque vendor code, don't deep-dive into it.

## Time (`src/time/`)
`TimeManager` (`time_manager.h/.cpp`) — fixed-frame-rate timing/delta-time/frame counting via
`std::chrono`. Used identically by both the client and server main loops.

## File I/O (`src/file_io/`)
- `vfs.h` (`ME::Vfs`, `ME::FileRoot { Resources, Dlc }`): file access by root instead of raw paths; the only
  place mapping a root to a folder (`Utils::GetResourcesPath()` / `GetDlcPath()`, both next to the exe).
  `ReadText` (text mode), `GetFileSize` / `ReadBytes` (binary, caller-owned buffer), `WriteBytes` (creates
  folders), `RemoveFile`, `RemoveEmptyFolders`. `std::filesystem` calls must use the `std::error_code`
  overloads (exceptions are disabled; the throwing overloads would terminate).
- Parsers read through `Vfs` and parse from memory: `INIParser::Load/Parse` (`ini_parser.h`,
  `map<string, map<string,string>>`), `CSVParser::Load/Parse`, `dds_parser` (DDS textures, client-side,
  still builds its own path).

## Serialization (`src/serialization/`)
`ByteWriter` / `ByteReader`: bounds-checked binary write/read over caller-owned memory, native byte order,
`memcpy` (no unaligned casts). General purpose: network messages, binary files.

## Third-party (out of scope for deep documentation)
`third_party/json` vendors **cJSON** — the one real external dependency in this codebase,
used throughout for JSON parsing (texture atlases, animation clips, wave data, etc. — see
[client/CLAUDE.md](../client/CLAUDE.md)). Treat as opaque vendor code.
