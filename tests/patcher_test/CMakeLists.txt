cmake_minimum_required(VERSION 3.8)

project(PatcherTest)

add_executable(UnpackTest unpack_test.cpp)

target_compile_features(UnpackTest PRIVATE cxx_std_17)



target_link_libraries(UnpackTest archiver patcher)



add_test(UnpackTest_t UnpackTest "e:\\\\")



set_tests_properties(UnpackTest_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")