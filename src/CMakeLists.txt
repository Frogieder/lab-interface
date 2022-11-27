add_subdirectory(misc)
add_subdirectory(menu)
add_subdirectory(knob)

add_executable(lab_interface
        main.cpp
#        misc/miscellaneous_functions.cpp
        )

target_link_libraries(lab_interface
        pico_stdlib
        pico_bootsel_via_double_reset
        hardware_i2c
        pico_multicore
        misc
        menu
        knob
        pico_ssd1306
        )

pico_enable_stdio_usb(lab_interface 1)
pico_enable_stdio_uart(lab_interface 0)

pico_add_extra_outputs(lab_interface)

target_include_directories (lab_interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
