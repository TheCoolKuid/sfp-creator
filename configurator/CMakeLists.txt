cmake_minimum_required(VERSION 3.8)

project(Configurator)

set(CPPS Configurator.cpp config/Config.cpp)

add_executable(Configurator ${CPPS})

target_compile_features(Configurator PRIVATE cxx_std_17)

target_include_directories(Configurator PRIVATE config)
target_link_libraries(Configurator archiver)