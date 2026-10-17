# The Village Shop: client and server, built on the Moksha engine. The `live` preset (CMakeUserPresets.json)
# builds the local engine clone instead of the pinned commit.

cmake_minimum_required(VERSION 3.31)

project(VillageShop LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The engine builds without exceptions; drop MSVC's default /EHsc so it doesn't clash with /EHs-c- (D9025).
if(MSVC)
    string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# The engine commit this game builds against. Bump it after pushing the engine changes the game needs.
set(MOKSHA_PIN 7520df8d9c23421970d72fc7a3fd2b4d71705444)

include(FetchContent)
FetchContent_Declare(moksha
    GIT_REPOSITORY https://github.com/amrit-choudhary/moksha.git
    GIT_TAG ${MOKSHA_PIN})
FetchContent_MakeAvailable(moksha)

add_subdirectory(client)
add_subdirectory(server)
