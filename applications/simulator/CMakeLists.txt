#find_package(qcustomplot REQUIRED)
include_directories (../../libraries/aircraft-simulator/headers)
#include_directories("${QCUSTOMPLOT_INCLUDE_DIR}")

add_executable (simulator main.cpp)
target_link_libraries (simulator AircraftSimulator)

# target_link_libraries (simulator AircraftSimulator ${QCUSTOMPLOT_LIBRARIES})
