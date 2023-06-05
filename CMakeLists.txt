cmake_minimum_required(VERSION 3.20)
project(yadro_csv_test)

set(CMAKE_CXX_STANDARD 14)

add_executable(yadro_csv_test main.cpp DataFrame.cpp DataFrame.h DFSupportStructs.h)
