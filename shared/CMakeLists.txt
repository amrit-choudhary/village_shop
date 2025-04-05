# This contains the shared code between client and server application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

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
