cmake_minimum_required(VERSION 3.17)
project(loadflow)

set(CMAKE_CXX_STANDARD 14)

add_executable(LFmain LFmain.cpp lf.cpp gs.cpp nr.cpp nrb.cpp fdl.cpp)