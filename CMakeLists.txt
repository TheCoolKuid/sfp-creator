cmake_minimum_required (VERSION 3.8)

project(SFP)


enable_testing()

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

add_subdirectory(external_libs)
add_subdirectory(archiver)
add_subdirectory(patcher)

add_subdirectory(configurator)

add_subdirectory(tests)

