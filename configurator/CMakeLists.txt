cmake_minimum_required(VERSION 3.8)

project(Configurator)

set(CPPS Configurator.cpp)

add_executable(Configurator ${CPPS})

target_compile_features(Configurator PRIVATE cxx_std_17)