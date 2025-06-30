cmake_minimum_required(VERSION 3.27)
cmake_policy(VERSION 3.27)

set(PICO_BOARD pico2_w CACHE STRING "Board type")
#set(PICO_PLATFORM rp2350-riscv CACHE STRING "Platform")
#set(PICO_COMPILER pico_riscv_gcc CACHE STRING "Compiler")
#set(PICO_GCC_TRIPLE riscv32-unknown-elf CACHE STRING "Target Triple")

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(i2c-lcd-display VERSION 0.1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()
include(contrib/drakos-pico-extensions/drakos-pico-extensions.cmake)

add_executable(i2c-lcd-display
        src/main.cxx
        src/i2c_lcd_display.cxx src/i2c_lcd_display.hxx
        src/entropy.c
)
target_link_libraries(i2c-lcd-display PRIVATE
        pico_stdlib pico_stdio pico_time pico_cyw43_arch_none pico_rand
        hardware_gpio hardware_i2c
        dpe_i2c dpe_led dpe_core
)
pico_enable_stdio_uart(i2c-lcd-display 0)
pico_enable_stdio_usb(i2c-lcd-display 1)
pico_add_extra_outputs(i2c-lcd-display)

