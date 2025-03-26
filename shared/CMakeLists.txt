# This contains the shared code between client and server application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(VillageShop_Shared STATIC ${SOURCES})

target_include_directories(VillageShop_Shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
