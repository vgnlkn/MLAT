cmake_minimum_required(VERSION 3.10)
project(MLAT VERSION 0.1)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(CheckCXXCompilerFlag)

add_subdirectory(applications)
add_subdirectory(libraries)

CHECK_CXX_COMPILER_FLAG ("-std=c++20" COMPILER_HAVE_CPP20)
if (COMPILER_HAVE_CPP20)
	set(CMAKE_CXX_STANDARD 20)
endif(COMPILER_HAVE_CPP20)

if (MSVC_VERSION GREATER_EQUAL "1900")
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_20)
    if (_cpp_20)
        message(STATUS "ADDING C++20")
        add_compile_options("/std:c++20")
    endif()
endif()