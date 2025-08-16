cmake_minimum_required(VERSION 3.10)
project(zlib VERSION 1.3.1 LANGUAGES C)

# Generate zconf.h from template
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein
               ${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY)

set(ZLIB_SRCS
    adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c gzread.c gzwrite.c
    infback.c inffast.c inflate.c inftrees.c trees.c uncompr.c zutil.c
)
set(ZLIB_PRIVATE_HDRS
    crc32.h deflate.h gzguts.h inffast.h inffixed.h inflate.h inftrees.h trees.h zutil.h
)
set(ZLIB_PUBLIC_HDRS
    zlib.h
    ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
)

add_library(zlibstatic STATIC
    ${ZLIB_SRCS}
    ${ZLIB_PUBLIC_HDRS}
    ${ZLIB_PRIVATE_HDRS}
)

target_include_directories(zlibstatic
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Produce the conventional name (libz.a / z.lib)
set_target_properties(zlibstatic PROPERTIES
    OUTPUT_NAME z
    POSITION_INDEPENDENT_CODE ON
)

# Canonical alias for consumers (libpng can link to this)
add_library(ZLIB::ZLIB ALIAS zlibstatic)