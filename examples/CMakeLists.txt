cmake_minimum_required(VERSION 3.8)

project(archive_example)

set(CPPS archive.cpp)

add_executable(archive_example ${CPPS})

target_compile_features(archive_example PRIVATE cxx_std_17)
target_link_libraries(archive_example lz4_static)