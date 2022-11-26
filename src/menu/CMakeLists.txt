add_library(menu menu.cpp)
target_link_libraries(menu pico_stdlib hardware_i2c pico_ssd1306)