cmake_minimum_required(VERSION 3.6)
project(washer_monitor)

set(CMAKE_CXX_STANDARD 11)

message("Test: $ENV{HOME}")

set(PIO $ENV{HOME}/.platformio)
set(PIO_ENVS .pioenvs/huzzah)
set(PIO_LIBDEPS .piolibdeps)
set(PIO_FRAMEWORK ${PIO}/packages/framework-arduinoespressif8266)

include_directories(${PIO_FRAMEWORK}/cores/esp8266)
include_directories(${PIO_FRAMEWORK}/libraries/ESP8266WiFi/src)
include_directories(${PIO_LIBDEPS}/MQTT_ID617/src)
include_directories(${PIO_LIBDEPS}/SimpleTimer_ID419)
include_directories(lib/util.embedded/src)

# platformio-style file discovery
file(GLOB_RECURSE pio_sources src/*.cpp)

set(SOURCE_FILES
        lib/readme.txt
        src/secrets.h
        ${pio_sources}
        )

add_executable(washer_monitor ${SOURCE_FILES})
