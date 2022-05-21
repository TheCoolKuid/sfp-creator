cmake_minimum_required(VERSION 3.8)

project(archiver)

set(CPPS archiver.cpp)

add_library(archiver SHARED ${CPPS})
add_library(archiver_static STATIC ${CPPS})

target_compile_features(archiver PRIVATE cxx_std_17)
target_compile_features(archiver_static PRIVATE cxx_std_17)

target_link_libraries(archiver lz4)
target_link_libraries(archiver_static lz4_static)
