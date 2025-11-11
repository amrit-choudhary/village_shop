# This contains the shared code between client and server application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add C sources (e.g. cJSON.c) to the build as well.
file(GLOB_RECURSE CSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list(APPEND SOURCES ${CSOURCES})

add_library(VillageShop_Shared STATIC ${SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Shared PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Shared PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Shared PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_include_directories(VillageShop_Shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(
    VillageShop_Shared
    PRIVATE
    VillageShop_Logging
    )
