cmake_minimum_required(VERSION 3.10)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../applications/utilities/)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../math/)

file(GLOB SIMULATOR_HEADERS headers/*.h)
file(GLOB SIMULATOR_SOURCES src/*.cpp)

add_library (AircraftSimulator ${SIMULATOR_HEADERS} ${SIMULATOR_SOURCES})
target_link_libraries (AircraftSimulator Math)
