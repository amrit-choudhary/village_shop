# CMake for the whole project.

cmake_minimum_required(VERSION 3.31)

project(VilageShop VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable RTTI and Exceptions globally
if (MSVC)
    add_compile_options(/GR-)
    add_compile_options(/EHsc)
    add_compile_options(/Wall)
    add_compile_options(/Qspectre-)
    add_compile_options(/wd5045)
    add_compile_options(/wd4820)
    add_compile_definitions(_HAS_EXCEPTIONS=0)
else()
  add_compile_options(-fno-rtti -fno-exceptions)
endif()

file(GLOB_RECURSE CLIENT_SHARED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/client/src/*.cpp")
list(REMOVE_ITEM CLIENT_SHARED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/client/src/main.cpp")
set(CLIENT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/client/src CACHE INTERNAL "Client include directory")

add_subdirectory(logging)
add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)
#add_subdirectory(client_tests)
add_subdirectory(client_package)
