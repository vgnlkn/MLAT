include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)

add_compile_definitions(STOCK_PATH="${CMAKE_CURRENT_SOURCE_DIR}/plotter/plotter.txt")
add_executable (simulator 
	main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plotter/plotter.h
	${CMAKE_CURRENT_SOURCE_DIR}/plotter/plotter.cpp
)
target_link_libraries (simulator AircraftSimulator)
