add_library(sudokulib sudoku.c sudoku.h)
target_include_directories(sudokulib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
