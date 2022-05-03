cmake_minimum_required(VERSION 3.8)

project(archiver)

set(CPPS archiver.cpp)

add_library(archiver STATIC ${CPPS})

target_compile_features(archiver PRIVATE cxx_std_17)

target_link_libraries(archiver lz4_static)