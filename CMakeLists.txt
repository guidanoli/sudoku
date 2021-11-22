cmake_minimum_required(VERSION 3.16.3)
project(sudoku)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(src)
add_subdirectory(tests)
