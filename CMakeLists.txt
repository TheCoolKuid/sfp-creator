cmake_minimum_required (VERSION 3.8)

project(SFP)

option(BUILD_SHARED_LIBS "" ON)

add_subdirectory(configurator ./build)
add_subdirectory(archiver ./build/archiver)
add_subdirectory(lz4/lz4/build/cmake)
#add_subdirectory(lz4/lz4/examples)
add_subdirectory(examples ./build/examples)