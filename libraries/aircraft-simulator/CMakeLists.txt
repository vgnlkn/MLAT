cmake_minimum_required(VERSION 3.25)

include_directories (headers/ src/)

add_library (AircraftSimulator
    headers/*.h
    src/*.cpp
)
