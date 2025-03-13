# CMake for the server application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_Server ${SOURCES})