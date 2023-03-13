cmake_minimum_required(VERSION 3.15)
project(MLAT VERSION 0.1)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
add_subdirectory(applications)
add_subdirectory(libraries)

