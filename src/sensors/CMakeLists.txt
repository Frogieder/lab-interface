add_library(sensors sensors.cpp)
target_link_libraries(sensors pico_stdlib hardware_i2c)