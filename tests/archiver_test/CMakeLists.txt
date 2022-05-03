cmake_minimum_required(VERSION 3.8)

project(ArchiverTest)

add_executable(ArchiverTest archiver_test_compression.cpp)
add_executable(ArchiverTestGenerate archiver_test_generate.cpp)

target_compile_features(ArchiverTest PRIVATE cxx_std_17)
target_compile_features(ArchiverTestGenerate PRIVATE cxx_std_17)

target_link_libraries(ArchiverTest archiver)
target_link_libraries(ArchiverTestGenerate archiver)

add_test(ArchiverTestt_t ArchiverTest)
add_test(ArchiverTestGenerate_t ArchiverTestGenerate)

set_tests_properties(ArchiverTestt_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(ArchiverTestGenerate_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")