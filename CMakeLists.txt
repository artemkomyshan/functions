project(functions)
cmake_minimum_required(VERSION 2.6)
enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")

include_directories(include)


#add_library(tests_src
#    test/shared_function.cpp
#    test/call_once_silent.cpp)

add_executable(test_function test/test.cpp)

#target_link_libraries(test_function PRIVATE tests_src)

add_test(test_function test_function)