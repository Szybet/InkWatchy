# wifi_tool
# build_src_filter =
# ${env:min.build_src_filter}
# +<**/wifiTool/*>

# apple joke
# build_src_filter =
# ${env:min.build_src_filter}
# +<**/EvilAppleJuice-ESP32/*>
# -<**/EvilAppleJuice-ESP32/src/main.cpp>

# max
# build_src_filter =
# ${env:min.build_src_filter}
# +<**/EvilAppleJuice-ESP32/*>
# -<**/EvilAppleJuice-ESP32/src/main.cpp>
# +<**/wifiTool/*>

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
)

if("${PIO_ENV_OUTPUT}" STREQUAL "min")
    message(STATUS "get_pio_env output is 'min'")

    # min
    # build_src_filter =
    # ${env.build_src_filter}
    # -<**/wifiTool/*>
    # -<**/EvilAppleJuice-ESP32/*>
    list(FILTER APP_SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/other/wifiTool/*" )
    list(FILTER APP_SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/other/EvilAppleJuice-ESP32/*" )
endif()

message(STATUS "App sources: ${APP_SOURCES}")

idf_component_register(SRCS ${APP_SOURCES})