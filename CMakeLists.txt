cmake_minimum_required(VERSION 3.27)
project(gg_bin)

set(CMAKE_CXX_STANDARD 17)

add_executable(gg_bin
        main.cpp
        json_parser/json.cpp
        utility_functions.cpp
        node_info.h)



