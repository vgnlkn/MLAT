cmake_minimum_required(VERSION 3.10)

include_directories (. tests/)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../applications/utilities/)

file(GLOB MATH_HEADERS *.h)
file(GLOB MATH_SOURCES src/*.cpp)

add_library (Math
    ${MATH_HEADERS}
    ${MATH_SOURCES}
    ../../applications/utilities/defines.h
)

file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Starting tests...")
    add_executable(test_math ${TESTS})

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
