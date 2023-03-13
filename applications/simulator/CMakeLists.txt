include_directories (../../libraries/aircraft-simulator/headers)

add_executable (simulator main.cpp)
target_link_libraries (simulator AircraftSimulator)
