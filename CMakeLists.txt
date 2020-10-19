cmake_minimum_required(VERSION 3.16)
project(ParallelMergeSort)

set(CMAKE_CXX_STANDARD 14)

link_libraries(pthread)

add_executable(ParallelMergeSort parallelmergesort.c parallelmergesort.h)
