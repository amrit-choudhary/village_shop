# CMake for the testing framework.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(VillageShop_Client_Tests ${SOURCES} ${CLIENT_SHARED_SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Client_Tests PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

# Link libraries
target_link_libraries(
    VillageShop_Client_Tests
    PRIVATE
    VillageShop_Logging
    )

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)
    if (NOT APPLICATION_SERVICES_FRAMEWORK)
        message(FATAL_ERROR "ApplicationServices framework not found")
    endif()
    target_link_libraries(VillageShop_Client_Tests PRIVATE ${APPLICATION_SERVICES_FRAMEWORK})
endif()

# Include Client code.
target_include_directories(VillageShop_Client_Tests PRIVATE ${CLIENT_INCLUDE_DIR})

## Post build.

set(RES_SOURCE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resources")

if(APPLE)
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client_tests/resources")
else()
    set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client_tests/Debug/resources")
endif()


add_custom_command(
    TARGET VillageShop_Client_Tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_SOURCE_FOLDER} ${RES_DESTINATION_FOLDER}
    COMMENT "Copying resources folder to build directory"
)
