cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

file(GLOB FILES *.cpp *.hpp)

add_library(main ${FILES})

target_include_directories(main PUBLIC ../include/)
target_link_libraries(main pico_stdlib)

target_link_libraries(main
        hardware_spi
        hardware_i2c
        hardware_uart
        hardware_pwm
        hardware_timer
        )

