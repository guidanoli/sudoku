add_library(sudokulib sudoku.c sudoku.h)
target_include_directories(sudokulib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sudokuaux sudokuaux.c sudokuaux.h)
target_include_directories(sudokuaux PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(sudokuaux sudokulib)

add_executable(sudokusolver sudokusolver.c)
target_link_libraries(sudokusolver sudokulib sudokuaux)

add_executable(randomsudoku randomsudoku.c)
