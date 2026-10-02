# content_server/

Content server: a small TCP service that delivers config/tuning files to game clients. Separate from the
UDP game server (`server/`). See root [CLAUDE.md](../CLAUDE.md) and [shared/CLAUDE.md](../shared/CLAUDE.md)
for the networking library it is built on.

## What it does
- At startup `ContentStore` (`src/content_store.*`) reads `dlc/manifest.json` and loads every listed file
  into memory. Missing or too-large files are logged and skipped (clients then can't finish syncing). An
  invalid or missing manifest stops the server. Content is a snapshot: edit files, bump versions, restart.
- `src/main.cpp`: single-threaded polling loop on port `9311` (`resources/config/settings.ini`, `port`),
  up to 32 clients (`MAX_CLIENTS`, static array), 1 ms sleep when idle. `TimeManager` is used only as a clock.
- `ClientConnection` (`src/client_connection.*`) is one client: socket, 4 KB request buffer, and the reply
  being sent (small header buffer + body sent straight from `ContentStore` memory, no copy). One request at a
  time; partial sends continue on later passes. Closes on invalid frames, unknown verbs, wrong version, and
  after 30 s without progress (`IDLE_TIMEOUT_SECONDS`).
- `GET_FILE` only finds files listed in the manifest and loaded at startup, so a request path never reaches
  the disk. Manifest names are validated by `ContentManifest::IsSafePath`.

## Protocol
Defined in `shared/src/net/content_protocol.h` (`ME::Net::ContentProtocol`), framed by
`shared/src/net/message_framing.h` (`u32 length | u8 version | u8 verb | payload`, native byte order):
`GET_MANIFEST` → `MANIFEST` (raw manifest.json text); `GET_FILE` (u16 path length, path, reserved u32 range
offset/length) → `FILE` (path + bytes) or `FILE_NOT_FOUND`. Max message 1 MB.

## dlc/ folder
`dlc/manifest.json`: `{ "files": [ { "name": "config/economy.json", "version": 1 }, ... ] }`. Names are
relative to `dlc/` with `/` separators; nested folders are fine. Versions are whole numbers bumped by hand:
change a file without bumping its version and clients won't download it. The repo's `dlc/` holds sample files.

## Running
Not copied by CMake: put `resources/` and `dlc/` next to the binary. Pi:
`cmake -B build && cmake --build build --target VillageShop_ContentServer -j2`, then
`cp -r content_server/resources content_server/dlc build/content_server/`. Stop with Ctrl+C (Ctrl+Z only
pauses it and keeps the port bound).

## Client side
`client/src/net/content_client.*` (see [client/CLAUDE.md](../client/CLAUDE.md)).
