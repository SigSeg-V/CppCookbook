cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppCookbookApp)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(CppCookbookApp
        src/main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/CppCookbookLib)
target_link_libraries(CppCookbookApp CppCookbookLib)

install(TARGETS CppCookbookApp RUNTIME DESTINATION bin)