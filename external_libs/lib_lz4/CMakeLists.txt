cmake_minimum_required (VERSION 3.8)

project(lib_lz4)

set(LZ4_SRC lz4.c lz4file.c lz4frame.c lz4hc.c xxhash.c)

add_library(lz4 SHARED ${LZ4_SRC})
add_library(lz4_static STATIC ${LZ4_SRC})