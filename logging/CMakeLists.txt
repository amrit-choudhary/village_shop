# Cmake for the logging shared dynamic library.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(VillageShop_Logging 
    SHARED 
    ${SOURCES}
)

target_include_directories(VillageShop_Logging
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(VillageShop_Logging 
    PROPERTIES
    VERSION 0.0.1
    SOVERSION 0.0.1
)

if(WIN32)
    target_compile_definitions(VillageShop_Logging PRIVATE "LOGGING_EXPORTS")
endif()
