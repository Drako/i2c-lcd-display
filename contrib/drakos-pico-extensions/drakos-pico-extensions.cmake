add_library(dpe_core INTERFACE)
target_sources(dpe_core INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/core/include/dpe/flags.hxx"
)
target_include_directories(dpe_core INTERFACE "${CMAKE_CURRENT_LIST_DIR}/core/include")

add_library(dpe_led INTERFACE)
target_sources(dpe_led INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/led/include/dpe/led.hxx"
)
target_include_directories(dpe_led INTERFACE "${CMAKE_CURRENT_LIST_DIR}/led/include")
target_link_libraries(dpe_led INTERFACE pico_stdlib hardware_gpio)

add_library(dpe_i2c INTERFACE)
target_sources(dpe_i2c INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/i2c/src/i2c_bus.cxx" "${CMAKE_CURRENT_LIST_DIR}/i2c/include/dpe/i2c_bus.hxx"
)
target_include_directories(dpe_i2c INTERFACE "${CMAKE_CURRENT_LIST_DIR}/i2c/include")
target_link_libraries(dpe_i2c INTERFACE pico_stdlib hardware_gpio hardware_i2c)
