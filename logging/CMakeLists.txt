# Logging library.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(VillageShop_Logging STATIC ${SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Logging PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Logging PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Logging PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_include_directories(VillageShop_Logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
