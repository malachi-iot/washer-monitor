cmake_minimum_required(VERSION 3.6)
project(washer_monitor)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lib/readme.txt
        src/main.cpp)

add_executable(washer_monitor ${SOURCE_FILES})