# The Village Shop

<p align="center">
<img src="/screenshots/2025_05_18.png" width="700px" />
</p>

## Introduction
This is a C++ game about establishing a profitable shop in a functioning village. Villagers will have multitudes of demands like food, shelter, luxury goods etc. and it will be your choice to venture in a business that you think will make you the most profit.

## Technical
- This is a pure C++ multiplayer game with no external dependencies. Right now, it runs minimally on the console. However, gradually different GUIs will be added.
- Every person in the village will be simulated and will behave like a real person. They will have their needs and will go to shops to buy items.
- You will have AI competitiors that will open shops with same or different items.
- Multiplayer mode will also be available where players can compete with each other in the same village simulation.
- The village will itself grow or shrink depending on how much of it's needs are satisfied.

## Project
### Folders
```
shared/                Code that will be shared by client and server.

client/                Folder for client application.
  /doc                 Technical design documents.
  /src                 Source code for client application.
  /resources           Game assets.
  /external            External libraries and plugins.

server/                Folder for server application.
  /doc                 Technical design documents.
  /src                 Source for server application.
  /resources           Game assets.
  /external            External libraries and plugins.

client_tests/          Project containing unit tests for client code.

logging/               A minimal logging library.

automation/            Automation and test scripts.

design/                Game design documents.

```

### Development
- This is developed in C++ language with CMake for builds. VS Code is the editor and its compiled using clang.
- To run this project, simply dowload this from github and run CMkakeList.txt files from client or server folder.
