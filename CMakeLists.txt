cmake_minimum_required(VERSION 3.21)
project(PQWA C)

set(CMAKE_C_STANDARD 99)

add_executable(PQWA main.c WaveformSample.c WaveformSample.h IO.h IO.c)
target_link_libraries(PQWA m)