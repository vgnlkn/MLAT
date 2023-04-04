include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/math)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)

add_compile_definitions(STOCK_PATH1="${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/plotter.txt")
add_compile_definitions(STOCK_PATH2="${CMAKE_CURRENT_SOURCE_DIR}/plotter.txt")
add_executable (simulator
	main.cpp
        ../utilities/defines.h)
target_link_libraries (simulator AircraftSimulator)
target_link_libraries (simulator Math)
