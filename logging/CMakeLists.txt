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

## POST BUILD

# Copy dll of logging library to client

if(APPLE)
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/libVillageShop_Logging.dylib")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/")
else()
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/Debug/VillageShop_Logging.dll")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/Debug/")
endif()

add_custom_command(
    TARGET VillageShop_Logging
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${RES_SOURCE_FILE} ${RES_DESTINATION_FOLDER}
    COMMENT "Copying logging dll to client build directory"
)

# Copy dll of logging library to client_tests

if(APPLE)
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/libVillageShop_Logging.dylib")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client_tests/")
else()
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/Debug/VillageShop_Logging.dll")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client_tests/Debug/")
endif()

add_custom_command(
    TARGET VillageShop_Logging
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${RES_SOURCE_FILE} ${RES_DESTINATION_FOLDER}
    COMMENT "Copying logging dll to clinet_testsbuild directory"
)
