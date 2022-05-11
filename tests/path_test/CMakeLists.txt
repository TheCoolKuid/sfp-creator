cmake_minimum_required(VERSION 3.8)

project(PathTestT)

add_executable(PathTestT path_append_test.cpp)

target_compile_features(PathTestT PRIVATE cxx_std_17)

add_test(PathTest_Normal_t PathTestT "C:" "C:/dir1/dir2/file.ext" "c" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSep_t PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "c" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArg_t PathTestT "C:" "C:/dir1/dir2/file.ext" "c" "/dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArgInPath_t PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "c" "/dir1" "dir2" "file.ext")

add_test(PathTest_Normal_t_string PathTestT "C:" "C:/dir1/dir2/file.ext" "s" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSep_t_string PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "s" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArg_t_string PathTestT "C:" "C:/dir1/dir2/file.ext" "s" "/dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArgInPath_t_string PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "s" "/dir1" "dir2" "file.ext")

add_test(PathTest_Normal_t_path PathTestT "C:" "C:/dir1/dir2/file.ext" "p" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSep_t_path PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "p" "dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArg_t_path PathTestT "C:" "C:/dir1/dir2/file.ext" "p" "/dir1" "dir2" "file.ext")
add_test(PathTest_WithSepInArgInPath_t_path PathTestT "C:\\\\" "C:\\\\dir1/dir2/file.ext" "p" "/dir1" "dir2" "file.ext")

add_test(PathTest_Add_ext PathTestT "C:/dir1/dir2/file" "C:/dir1/dir2/file.ext" "c" ".ext")
add_test(PathTest_Add_ext_s PathTestT "C:/dir1/dir2/file" "C:/dir1/dir2/file.ext" "s" ".ext")

set_tests_properties(PathTest_Normal_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSep_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArg_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArgInPath_t PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(PathTest_Normal_t_string PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSep_t_string PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArg_t_string PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArgInPath_t_string PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(PathTest_Normal_t_path PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSep_t_path PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArg_t_path PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_WithSepInArgInPath_t_path PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")

set_tests_properties(PathTest_Add_ext PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")
set_tests_properties(PathTest_Add_ext_s PROPERTIES
PASS_REGULAR_EXPRESSION "Pass"
FAIL_REGULAR_EXPRESSION "Fail")