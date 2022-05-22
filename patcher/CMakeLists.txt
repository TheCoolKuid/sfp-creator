cmake_minimum_required(VERSION 3.8)

project(patcher)

set(CPPS main.cpp patcher.cpp)

add_executable(patcher ${CPPS})

target_compile_features(patcher PRIVATE cxx_std_17)

target_link_libraries(patcher ${CMAKE_CURRENT_SOURCE_DIR}/lz4_static.lib ${CMAKE_CURRENT_SOURCE_DIR}/archiver_static.lib)