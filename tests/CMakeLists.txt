set(INPUT "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
set(REAL_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/output.txt")
set(EXPECTED_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/output.txt")

add_test(NAME RunSolver COMMAND sudokusolver "${INPUT}" "${REAL_OUTPUT}")

add_test(NAME CheckResults COMMAND "${CMAKE_COMMAND}" -E compare_files "${REAL_OUTPUT}" "${EXPECTED_OUTPUT}")
set_tests_properties(CheckResults PROPERTIES DEPENDS RunSolver)

add_test(NAME CleanUp COMMAND "${CMAKE_COMMAND}" -E remove "${REAL_OUTPUT}")
set_tests_properties(CleanUp PROPERTIES DEPENDS CheckResults)
