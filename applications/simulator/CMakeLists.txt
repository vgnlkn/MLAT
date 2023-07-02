include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/aircraft-simulator/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/math)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/plotter)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../utilities)


file(GLOB UTILITY_HEADERS ../utilities/*.h)
file(GLOB SIMULATOR_HEADERS headers/*.h)

add_executable (simulator
	main.cpp
	src/main_class.cpp
	${SIMULATOR_HEADERS}
	${UTILITY_HEADERS}
)

target_link_libraries (simulator AircraftSimulator)
target_link_libraries (simulator Math)

add_compile_definitions(STOCK_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../config.json")
