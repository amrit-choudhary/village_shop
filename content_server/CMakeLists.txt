# CMake for the content server application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_ContentServer ${SOURCES})

target_include_directories(VillageShop_ContentServer PRIVATE ${CMAKE_SOURCE_DIR})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_ContentServer PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_ContentServer PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_ContentServer PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_link_libraries(
    VillageShop_ContentServer
    PRIVATE
    VillageShop_Shared
    VillageShop_Logging
    )
