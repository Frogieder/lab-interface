cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(lab_interface C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
pico_sdk_init()

add_subdirectory(pico-ssd1306)

add_subdirectory(src)
