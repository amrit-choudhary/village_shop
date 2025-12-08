# CMake for the client application.
include(CMakePrintHelpers)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm"
)

if(APPLE)
    # Add metal headers.
    add_subdirectory(third_party/metal)
endif()

# Add miniaudio
add_subdirectory(third_party/miniaudio-0.11.23)

# Executable
add_executable(VillageShop_Client ${SOURCES})

# Define that this is a Windows application not a CLI.
if(WIN32)
    set_target_properties(VillageShop_Client PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

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
    VillageShop_Shared
    miniaudio
    )

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)   
    target_link_libraries(VillageShop_Client PRIVATE ${APPLICATION_SERVICES_FRAMEWORK})
    target_link_libraries(VillageShop_Client PRIVATE METAL_LIB)
endif()

if(WIN32)
    # Link against Winsock library
    target_link_libraries(VillageShop_Client PRIVATE Ws2_32)
    # Link against DirectX 12 libraries
    target_link_libraries(VillageShop_Client PRIVATE d3d12 dxgi d3dcompiler)
endif()

#End link libraries

## POST BUILD

# Copy client resources folder
set(RES_SOURCE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resources")

# Use the current directory's binary dir so this works for single-config and multi-config generators
set(RES_DESTINATION_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DESTINATION_FOLDER}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RES_SOURCE_FOLDER}" "${RES_DESTINATION_FOLDER}"
    COMMENT "Copying resources to build directory (manual target)"
)
