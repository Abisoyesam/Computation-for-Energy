cmake_minimum_required(VERSION 3.17)
project(transstab)

set(CMAKE_CXX_STANDARD 14)

add_executable(transstab TSmain.cpp ts.cpp Tsderive.cpp)