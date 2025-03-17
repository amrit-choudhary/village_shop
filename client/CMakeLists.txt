# CMake for the client application.
include(CMakePrintHelpers)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_Client ${SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Client PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Client PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Client PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

# Link libraries
target_link_libraries(
    VillageShop_Client
    PRIVATE
    VillageShop_Logging
    )

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)
    if (NOT APPLICATION_SERVICES_FRAMEWORK)
        message(FATAL_ERROR "ApplicationServices framework not found")
    endif()    
    target_link_libraries(VillageShop_Client PRIVATE ${APPLICATION_SERVICES_FRAMEWORK})
endif()
#End link libraries


## POST BUILD

# Copy client resources folder
set(RES_SOURCE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resources")

if(APPLE)
set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/resources")
else()
set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/Debug/resources")
endif()

add_custom_command(
    TARGET VillageShop_Client
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_SOURCE_FOLDER} ${RES_DESTINATION_FOLDER}
    COMMENT "Copying resources folder to build directory"
)

# Copy dll of logging library

if(APPLE)
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/libVillageShop_Logging.dylib")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/")
else()
    set(RES_SOURCE_FILE "${CMAKE_BINARY_DIR}/logging/Debug/VillageShop_Logging.dll")
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/Debug/")
endif()

add_custom_command(
    TARGET VillageShop_Client
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${RES_SOURCE_FILE} ${RES_DESTINATION_FOLDER}
    COMMENT "Copying logging dll to build directory"
)