include_directories (../../libraries/math/)

add_executable (test-application main.cpp)
target_link_libraries (test-application Math)
