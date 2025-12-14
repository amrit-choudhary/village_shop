# CMake for the client package application.

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VillageShop_Package ${SOURCES})

# Set preprocessor options.
if(WIN32)
    target_compile_definitions(VillageShop_Package PRIVATE VG_WIN)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(VillageShop_Package PRIVATE VG_LINUX)
elseif(APPLE)
    target_compile_definitions(VillageShop_Package PRIVATE VG_MAC)
endif()
# End Set preprocessor options.

target_link_libraries(
    VillageShop_Package
    PRIVATE
    VillageShop_Shared
)

## POST BUILD

# Copy client resources folder
set(RES_SOURCE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resources")

# Use the current directory's binary dir so this works for single-config and multi-config generators
set(RES_DESTINATION_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(TARGET VillageShop_Package
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DESTINATION_FOLDER}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RES_SOURCE_FOLDER}" "${RES_DESTINATION_FOLDER}"
    COMMENT "Copying resources to output directory"
)
