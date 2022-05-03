cmake_minimum_required (VERSION 3.8)

project(SFP)


enable_testing()

#add_subdirectory(lz4/lz4/build/cmake)
add_subdirectory(archiver)

add_subdirectory(configurator)

add_subdirectory(tests)

