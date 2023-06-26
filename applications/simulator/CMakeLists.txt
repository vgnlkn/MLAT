include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/math)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../utilities)

add_executable (simulator
	main.cpp
    ../utilities/defines.h
	src/main_class.cpp
	headers/main_class.h
	../utilities/json_reader.h
	../utilities/interface_reader.h)

target_link_libraries (simulator AircraftSimulator)
target_link_libraries (simulator Math)

target_compile_definitions(simulator PUBLIC STOCK_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../config.json")
target_compile_definitions(simulator PUBLIC PLOTTER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/")

