idf_component_register(
    INCLUDE_DIRS "include"
)

add_dependencies(${COMPONENT_LIB} rust_build)

add_library(rusty STATIC IMPORTED)
set_target_properties(rusty PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/librusty.a"
)

target_link_libraries(${COMPONENT_LIB} INTERFACE rusty)