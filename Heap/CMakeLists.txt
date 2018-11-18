cmake_minimum_required(VERSION 3.12)
project(Heap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(google-tests/googletest-master)
include_directories(google-tests/googletest-master/googletest/include)
include_directories(google-tests/googletest-master/googlemock/include)

add_executable(Heap main.cpp Tests/test.cpp Tests/Vector.h Tests/Heap.h)
target_link_libraries(Heap gtest gtest_main)