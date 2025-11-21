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

# Add zlib
add_subdirectory(third_party/zlib-1.3.1)

# Add libpng
# Disable all libpng install/export logic
set(SKIP_INSTALL_ALL          ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_EXPORT       ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_LIBRARIES    ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_HEADERS      ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_FILES        ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_PROGRAMS     ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_CONFIG_FILE  ON CACHE BOOL "" FORCE)

set(PNG_SHARED OFF  CACHE BOOL "" FORCE)
set(PNG_STATIC  ON  CACHE BOOL "" FORCE)
set(PNG_TESTS   OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS   OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/libpng-1.6.47)

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
    png_static
    ZLIB::ZLIB
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

if(APPLE)
set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/resources")
else()
set(RES_DESTINATION_FOLDER "${CMAKE_BINARY_DIR}/client/Debug/resources")
endif()

add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DESTINATION_FOLDER}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RES_SOURCE_FOLDER}" "${RES_DESTINATION_FOLDER}"
    COMMENT "Copying resources to build directory (manual target)"
)
