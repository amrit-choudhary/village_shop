# Unit tests for client-side code (currently the shared library parts the client uses).

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_Client_Tests ${SOURCES})

target_include_directories(VillageShop_Client_Tests PRIVATE ${CMAKE_SOURCE_DIR})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_link_libraries(
    VillageShop_Client_Tests
    PRIVATE
    VillageShop_TestFramework
    VillageShop_Shared
    VillageShop_Logging
    )
