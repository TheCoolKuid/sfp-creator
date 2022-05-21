cmake_minimum_required(VERSION 3.8)

project(patcher)

set(CPPS main.cpp patcher.cpp)

add_executable(patcher ${CPPS})

target_compile_features(patcher PRIVATE cxx_std_17)

target_link_libraries(patcher lz4 archiver)