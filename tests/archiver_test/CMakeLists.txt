cmake_minimum_required(VERSION 3.8)

project(ArchiverTest)

add_executable(ArchiverTest archiver_test_compression.cpp)
add_executable(ArchiverTestGenerate archiver_test_generate.cpp)
add_executable(CRC32OfFile crc_of_file.cpp)

target_compile_features(ArchiverTest PRIVATE cxx_std_17)
target_compile_features(ArchiverTestGenerate PRIVATE cxx_std_17)
target_compile_features(CRC32OfFile PRIVATE cxx_std_17)

target_link_libraries(ArchiverTest archiver)
target_link_libraries(ArchiverTestGenerate archiver)
target_link_libraries(CRC32OfFile archiver)

add_test(ArchiverTestt_t ArchiverTest)
add_test(ArchiverTestGenerate_t ArchiverTestGenerate)
add_test(CRC32OfFile_t CRC32OfFile)

set_tests_properties(ArchiverTestt_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(ArchiverTestGenerate_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(CRC32OfFile_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")