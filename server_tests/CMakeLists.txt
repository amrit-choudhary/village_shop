# Unit tests for server code. Compiles the server sources under test directly (the server has no library target).

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(SERVER_SOURCES_UNDER_TEST
    "${CMAKE_SOURCE_DIR}/server/src/db/score_db.cpp"
    )

add_executable(VillageShop_Server_Tests ${SOURCES} ${SERVER_SOURCES_UNDER_TEST})

target_include_directories(VillageShop_Server_Tests PRIVATE ${CMAKE_SOURCE_DIR})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Server_Tests PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Server_Tests PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Server_Tests PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_link_libraries(
    VillageShop_Server_Tests
    PRIVATE
    VillageShop_TestFramework
    VillageShop_Shared
    sqlite3
    )
