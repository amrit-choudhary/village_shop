# shared/

Code shared by client and server. See root [CLAUDE.md](../CLAUDE.md) for project-level context.
Everything here is hand-rolled (no STL containers used for game data, no external libs)
except where noted — this matches the project's "no external dependencies" intent, with
one real exception (cJSON, noted below).

## Networking / wire protocol (`src/net/`)
- `net_protocol.h` — wire format: 1 byte version, 1 byte verb (`Verb` is `uint8_t`), 1 byte
  clientID, then payload. `Verb` enum reserves ranges: System `0x00-0x1F`, Http `0x20-0x3F`,
  Matchmaking `0x40-0x5F`, Gameplay `0x60-0x7F`. `ConnectedClient`/`ConnectedServer` hold raw
  address/port/clientID.
- `net_packet.h/.cpp` — `Packet` base wraps a raw `uint8_t*` with a manual read/write cursor
  (`WriteByte/ReadByte/WriteString/ReadString/WriteFP/ReadFP`, direct pointer arithmetic and
  `strcpy`/`reinterpret_cast`, **no bounds checking, no endianness handling**). Fixed-size
  pool subclasses: `PacketSmall`(64) / `PacketMedium`(256) / `PacketBig`(1024) /
  `PacketHuge`(2048) bytes, each `new uint8_t[size]`.
- `net_utils.h/.cpp` — `GetVerbName(Verb)` debug helper only.
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
Hand-rolled parsers: `csv_parser`/`csv_data`, `ini_parser.h` (simple
`map<string, map<string,string>>` representation), `dds_parser` (DirectDraw Surface texture
reading, client-side use).

## Third-party (out of scope for deep documentation)
`third_party/json` vendors **cJSON** — the one real external dependency in this codebase,
used throughout for JSON parsing (texture atlases, animation clips, wave data, etc. — see
[client/CLAUDE.md](../client/CLAUDE.md)). Treat as opaque vendor code.
