cmake_minimum_required(VERSION 3.15)

include_directories (headers/ src/ tests/)

add_library (AircraftSimulator
    headers/aircraft.h
    src/aircraft.cpp
)

find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Starting tests...")
    add_executable(test_aircraft 
        tests/main.cpp
        tests/test_aircraft.cpp
    )

    target_link_libraries(test_aircraft 
        ${CMAKE_THREAD_LIBS_INIT} 
        GTest::GTest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        AircraftSimulator
    )
    enable_testing()
    add_test(NAME Tests COMMAND tests_aircraft)

endif (${GTest_FOUND})
