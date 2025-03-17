# CMake for the testing framework.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(VillageShop_Client_Tests ${SOURCES} ${CLIENT_SHARED_SOURCES})

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)
    if (NOT APPLICATION_SERVICES_FRAMEWORK)
        message(FATAL_ERROR "ApplicationServices framework not found")
    endif()
    target_link_libraries(VillageShop_Client_Tests PRIVATE ${APPLICATION_SERVICES_FRAMEWORK})
endif()

target_include_directories(VillageShop_Client_Tests PRIVATE ${CLIENT_INCLUDE_DIR})

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
