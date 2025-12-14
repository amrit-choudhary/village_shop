# CMake for the client package application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_Package ${SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Package PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Package PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Package PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_link_libraries(
    VillageShop_Package
    PRIVATE
    VillageShop_Shared
    )
