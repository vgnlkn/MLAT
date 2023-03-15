cmake_minimum_required(VERSION 3.15)

include_directories (. tests/)

add_library (Math
    vector.h
    equation_solver.h
    src/equation_solver.cpp
)

find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Starting tests...")
    add_executable(test_math 
        tests/main.cpp
        tests/test_vector.cpp
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
