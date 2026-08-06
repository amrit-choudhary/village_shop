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

server/                Folder for server application.
  /doc                 Technical design documents.
  /src                 Source for server application.
  /resources           Game assets.

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
  which builds `logging` -> `shared` -> `client` -> `server` -> `client_package` in order.
