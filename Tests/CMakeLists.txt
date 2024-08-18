cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppCookbookTests)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(CppCookbookTests
        ConceptsTest.cpp
)

enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(CppCookbookTests PRIVATE
        # Cookbook code
        CppCookbookLib

        # GTest suite
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(CppCookbookTests)