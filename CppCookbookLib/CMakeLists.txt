cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppCookbookLib)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(CppCookbookLib
        src/Concepts.cppm
)

target_sources(CppCookbookLib
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        src/Concepts.cppm
)

install(TARGETS CppCookbookLib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)