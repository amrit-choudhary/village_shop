# Custom finder that reuses the in-tree zlib target.
if(TARGET ZLIB::ZLIB)
    get_target_property(_z_incs ZLIB::ZLIB INTERFACE_INCLUDE_DIRECTORIES)
    set(ZLIB_INCLUDE_DIR "${_z_incs}")
    set(ZLIB_INCLUDE_DIRS "${_z_incs}")
    set(ZLIB_LIBRARIES ZLIB::ZLIB)
    set(ZLIB_VERSION_STRING "1.3.1")
    set(ZLIB_FOUND TRUE)
    return()
endif()

# Fallback (should not be needed)
include(${CMAKE_ROOT}/Modules/FindZLIB.cmake)