# The Village Shop

## Introduction
This is a C++ game about establishing a profitable shop in a functioning village. Villagers will have multitudes of demands like food, shelter, luxury goods etc. and it will be your choice to venture in a business that you think will make you the most profit.

## Technical
- This is a pure C++ game with no external dependencies. Right now, it runs minimally on the console. However, gradually different GUIs will be added.
- Every person in the village will be simulated and will behave like a real person. They will have their needs and will go to shops to buy items.
- You will have AI competitiors that will open shops with same or different items.
- Multiplayer mode will also be available where players can compete with each other in the same village simulation.
- The village will itself grow or shrink depending on how much of it's needs are satisfied.

## Project
### Folders
```
automation/            Automation and test scripts.

design/                Game design documents.

client/                Folder for client applications.
client/doc             Technical design documents.
client/src             Source code for client application.
client/resources       Game assets.
client/bin             Client builds folder.
client/external        External libraries and plugins.

client_tests/          Project containing unit tests for client code.

logging/               A minimal logging library.

server/                Folder for server application.
server/doc             Technical design documents.
server/src             Source for server application.
server/resources       Game assets.
server/bin             Server builds folder.
```

### Development
- This is developed in C++ language with CMake for builds. VS Code is the editor and its compiled using clang.
- To run this project, simply dowload this from github and run CMkakeList.txt files from client or server folder.
