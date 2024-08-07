set(GLOBAL_FUNCTIONS_SCRIPT "${CMAKE_SOURCE_DIR}/resources/tools/globalFunctions.sh")
set(LAUNCH_JSON_PATH "${CMAKE_SOURCE_DIR}/.vscode/launch.json")
execute_process(
    COMMAND bash -c "source ${GLOBAL_FUNCTIONS_SCRIPT} && get_pio_env ${LAUNCH_JSON_PATH}"
    OUTPUT_VARIABLE PIO_ENV_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "get_pio_env output: ${PIO_ENV_OUTPUT}") # SEND_ERROR, WARNING

file(GLOB_RECURSE APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/*.hpp
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/*.h
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/*.cpp
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/*.c
)

file(GLOB_RECURSE EXCLUDE_SOURCES
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/ArduinoJson/extras/*
    ${CMAKE_SOURCE_DIR}/.pio/libdeps/Watchy_2/open-meteo-arduino/src/main.cpp
)

list(REMOVE_ITEM APP_SOURCES ${EXCLUDE_SOURCES})

#message(FATAL_ERROR "App sources: ${APP_SOURCES}")

idf_component_register(SRCS ${APP_SOURCES} REQUIRES "arduino")