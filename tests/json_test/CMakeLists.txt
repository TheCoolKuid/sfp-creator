cmake_minimum_required(VERSION 3.8)

project(JSONTest)

add_executable(JSONObjTest json_test_obj.cpp)
add_executable(JSONArrTest json_test_arr.cpp)
add_executable(JSONObjInArrayTest json_test_obj_in_arr.cpp)

target_compile_features(JSONObjTest PRIVATE cxx_std_17)
target_compile_features(JSONArrTest PRIVATE cxx_std_17)
target_compile_features(JSONObjInArrayTest PRIVATE cxx_std_17)

add_test(JSONObjTest_t JSONObjTest "{\"string\" : \"test\", \"number\" : 123, \"bool\" : true}")
add_test(JSONArrTest_t JSONArrTest "[\"abc\", 12, true]")
add_test(JSONObjInArrayTest_t JSONObjInArrayTest "[{\"field\" : \"filed1\"}, {\"field\" : \"filed2\"}, {\"field\" : \"filed3\"}, {\"field\" : \"filed4\"}]")

set_tests_properties(JSONObjTest_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(JSONArrTest_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(JSONObjInArrayTest_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")