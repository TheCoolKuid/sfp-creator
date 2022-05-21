cmake_minimum_required(VERSION 3.8)

project(patcher)

set(CPPS patcher.cpp)

add_library(patcher STATIC ${CPPS})

target_compile_features(patcher PRIVATE cxx_std_17)

target_link_libraries(patcher lz4 archiver)