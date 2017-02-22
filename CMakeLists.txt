cmake_minimum_required(VERSION 3.6)
project(washer_monitor)

set(CMAKE_CXX_STANDARD 11)

message("Test: $ENV{HOME}")

set(PIO $ENV{HOME}/.platformio)
set(PIO_FRAMEWORK ${PIO}/packages/framework-arduinoespressif8266)

include_directories(${PIO_FRAMEWORK}/cores/esp8266)
include_directories(${PIO_FRAMEWORK}/libraries/ESP8266WiFi/src)

set(SOURCE_FILES
        lib/readme.txt
        src/main.cpp src/secrets.h)

add_executable(washer_monitor ${SOURCE_FILES})