cmake_minimum_required(VERSION 3.15)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../math/)

add_library (AircraftSimulator
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/aircraft.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/field.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/tower.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/plotter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aircraft.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/field.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tower.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/plotter.cpp
)
target_link_libraries (AircraftSimulator Math)

#find_package(GTest)
#if (${GTest_FOUND})
 #   message(STATUS "Starting tests...")
  #  add_executable(test_aircraft 
    #    tests/main.cpp
    #    tests/test_aircraft.cpp
   # )

    #target_link_libraries(test_aircraft 
    #    ${CMAKE_THREAD_LIBS_INIT} 
    #    GTest::GTest
    #    GTest::gtest_main
    #    GTest::gmock
    #    GTest::gmock_main
    #    AircraftSimulator
    #)
    #enable_testing()
    #add_test(NAME Tests COMMAND tests_aircraft)

#endif (${GTest_FOUND})
