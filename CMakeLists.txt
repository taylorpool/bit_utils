cmake_minimum_required(VERSION 3.10)
project(bit_utils)

add_library(bit_utils INTERFACE)
target_include_directories(bit_utils INTERFACE include)

option(BUILD_BIT_UTILS_TESTS "Build bit_utils tests" OFF)
if(BUILD_BIT_UTILS_TESTS)
    find_package(GTest REQUIRED)
    add_executable(test_bit_utils
        src/test/test_bit_utils.cpp)
    target_link_libraries(test_bit_utils PUBLIC
        bit_utils
        gtest
        gtest_main
        pthread)
endif()