# The Village Shop

<p align="center">
<img src="/screenshots/survivors_final.gif" width="700px" />
</p>

<p align="center">
<img src="/screenshots/2025_06_13.gif" width="500px" />
</p>

## Introduction
This is a C++ game about establishing a profitable shop in a functioning village. Villagers will have multitudes of demands like food, shelter, luxury goods etc. and it will be your choice to venture in a business that you think will make you the most profit.

## Technical
- This is a C++20 multiplayer game with minimal external dependencies (a few vendored
  third-party libraries — see below). It builds with native renderer backends (DX12 on
  Windows, Metal on Mac) plus an ASCII/CLI renderer for a console-only build.
- Every person in the village will be simulated and will behave like a real person. They will have their needs and will go to shops to buy items.
- You will have AI competitiors that will open shops with same or different items.
- Multiplayer mode will also be available where players can compete with each other in the same village simulation.
- The village will itself grow or shrink depending on how much of it's needs are satisfied.
- Backend services are separate programs that also run on Linux (tested on a Raspberry Pi 4):
  a UDP **game server** for live multiplayer, and a TCP **content server** that delivers
  config/tuning files to clients, so game data can change without shipping a new client.

## Project
### Folders
```
shared/                Code shared by client and server: networking protocol, math,
                        physics, RNG, data structures, file I/O parsers.
  /src                 Source code.
  /third_party         Vendored third-party libraries (e.g. cJSON).

client/                Folder for client application.
  /doc                 Technical design documents.
  /src                 Source code for client application.
  /resources           Game assets.
  /third_party         Vendored third-party libraries (miniaudio, Metal helper headers).

server/                Game server (UDP, live multiplayer).
  /doc                 Technical design documents.
  /src                 Source for server application.
  /resources           Server settings.
  /third_party         Vendored SQLite (persists the high score).

content_server/        Content server (TCP): serves the files listed in dlc/manifest.json;
                        clients download new or changed files at startup.
  /src                 Source for content server application.
  /resources           Server settings.
  /dlc                 Content to serve (manifest.json + files).

client_package/        Separate CLI tool that cooks/packages client/resources for
                        distribution (HLSL->CSO via dxc.exe, textures->DDS via texconv.exe).

client_tests/          Unit tests for client code (currently disabled in the root build).

logging/               A minimal logging library.

automation/            Local dev-helper scripts (formatting, codesigning, LOC counts,
                        a smoke test). Not a CI pipeline.

design/                Game design documents.

```

### Development
- This is developed in C++20 with CMake for builds.
- To run this project, download it from github and build via the root `CMakeLists.txt`,
  which builds `logging` -> `shared` -> `client` -> `server` -> `content_server` ->
  `client_package` in order.
- After adding or removing source files, re-run CMake configure (`cmake -B build`), since
  source lists are collected at configure time.
- The servers look for their `resources/` (and the content server for `dlc/`) next to the
  executable; copy those folders next to the built binaries before running. Default ports:
  game server UDP 9310, content server TCP 9311 (set in each `resources/config/settings.ini`).
- Building only the servers on Linux:
  `cmake -B build && cmake --build build --target VillageShop_Server VillageShop_ContentServer`.
