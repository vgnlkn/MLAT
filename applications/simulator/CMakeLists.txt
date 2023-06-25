include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/math)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../utilities)

add_executable (simulator
	main.cpp
    ../utilities/defines.h
	../utilities/src/main_class.cpp ../utilities/main_class.h)
target_link_libraries (simulator AircraftSimulator)
target_link_libraries (simulator Math)

target_compile_definitions(simulator PUBLIC STOCK_PATH_MLAT="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/mlat.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_REAL="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/real.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_FILTER="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/filter.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_SPEED_FILTER="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/speed_filter.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_SPEED_REAL="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/speed_real.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_ACCELERATION_FILTER="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/acceleration_filter.txt")
target_compile_definitions(simulator PUBLIC STOCK_PATH_ACCELERATION_REAL="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/acceleration_real.txt")
