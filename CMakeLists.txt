cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

add_executable(display "example/main.cpp" "src/LCDbase.cpp" "src/LCDbase.h" "src/LCDgeneric.cpp" "src/LCDgeneric.h")

target_link_libraries(display pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(display 1)
pico_enable_stdio_uart(display 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(display)