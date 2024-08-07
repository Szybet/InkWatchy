cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

file(GLOB SRC_DIRS
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2"
    "${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/ArduinoJson/extras/tests/catch"
)

foreach(DIR ${SRC_DIRS})
    include_directories(${DIR})
endforeach()

project(InkWatchy-personal)
