cmake_minimum_required(VERSION 3.10)

include_directories (. tests/)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../applications/utilities/)

add_library (Math
    vector.h
    equation_solver.h
    matrix.h
    src/equation_solver.cpp
        ../../applications/utilities/defines.h
	kalman_filter.h
        mlat_estimation.h
        src/mlat_estimation.cpp
        extended_evaluation.h src/extended_evaluation.cpp extended_filter.h ekf.h)

find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Starting tests...")
    add_executable(test_math 
        tests/main.cpp
        tests/test_vector.cpp
        tests/test_matrix.cpp
    )

    target_link_libraries(test_math 
        ${CMAKE_THREAD_LIBS_INIT} 
        GTest::GTest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        Math
    )
    enable_testing()
    add_test(NAME ExampleTest COMMAND test_math)

endif (${GTest_FOUND})
