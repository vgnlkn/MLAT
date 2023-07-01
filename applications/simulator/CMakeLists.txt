include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/math)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../utilities)


file(GLOB UTILITY_HEADERS ../utilities/*.h)

add_executable (simulator
	main.cpp
	src/main_class.cpp
	headers/main_class.h
	${UTILITY_HEADERS}
)

target_link_libraries (simulator AircraftSimulator)
target_link_libraries (simulator Math)

target_compile_definitions(simulator PUBLIC STOCK_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../config.json")
target_compile_definitions(simulator PUBLIC PLOTTER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../utilities/plotters/")

