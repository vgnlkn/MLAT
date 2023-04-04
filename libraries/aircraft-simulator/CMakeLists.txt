cmake_minimum_required(VERSION 3.10)

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
