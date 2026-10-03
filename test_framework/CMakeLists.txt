# Minimal unit test framework, linked by the test executables.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(VillageShop_TestFramework STATIC ${SOURCES})

target_include_directories(VillageShop_TestFramework PUBLIC ${CMAKE_SOURCE_DIR})
