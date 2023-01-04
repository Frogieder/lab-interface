add_library(sensors
        sensors.cpp
        builtin_clock.cpp
        AD22151.cpp
        )

target_link_libraries(sensors pico_stdlib hardware_i2c)