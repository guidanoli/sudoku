add_library(sudokulib sudoku.c sudoku.h)
target_include_directories(sudokulib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(sudokusolver sudokusolver.c)
target_link_libraries(sudokusolver sudokulib)

add_executable(randomsudoku randomsudoku.c)
