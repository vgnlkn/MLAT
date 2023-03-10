cmake_minimum_required(VERSION 3.15)
project(MLAT VERSION 0.1)

# потом конан настрою
#include(conanbuildinfo.cmake)
#set(CONAN_DISABLE_CHECK_COMPILER true)
#conan_basic_setup()

add_subdirectory(applications)
add_subdirectory(libraries)

