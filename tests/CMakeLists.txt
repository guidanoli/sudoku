add_executable(testsudoku testsudoku.c)
target_link_libraries(testsudoku sudokulib sudokuaux)
set(TESTFILE "${CMAKE_CURRENT_SOURCE_DIR}/boards.txt")
add_test(NAME Sudoku COMMAND testsudoku "${TESTFILE}")
