# CMake for the client application.
include(CMakePrintHelpers)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if(APPLE)
    # Add metal headers.
    add_subdirectory(third_party/metal)
endif()

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

target_link_libraries(
    VillageShop_Client
    PRIVATE
    VillageShop_Shared
    )

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)   
    target_link_libraries(VillageShop_Client PRIVATE ${APPLICATION_SERVICES_FRAMEWORK})
    target_link_libraries(VillageShop_Client PRIVATE METAL_LIB)
endif()

if(WIN32)
    # Link against Winsock library
    target_link_libraries(VillageShop_Client PRIVATE Ws2_32)
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
