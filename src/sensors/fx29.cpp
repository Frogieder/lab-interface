//
// Created by martin on 12/19/22.
//

#include "fx29.hpp"

bool FX29::init(Port port) {
    this->set_scale(1);
    this->port = port;
    init_done = true;
    return init_done;
}

int16_t FX29::get_raw_blocking() {
    union {
        uint8_t uint8;
        uint16_t uint16;
    } buffer{};
    while (true) {
        auto i2c_result = i2c_read_blocking_until(SENSOR_I2C, 0x28, &buffer.uint8, 2, false, make_timeout_time_ms(10));
        if (i2c_result == PICO_ERROR_GENERIC or i2c_result == PICO_ERROR_TIMEOUT)
            return ~0;
        if (!(buffer.uint16 & (1<<15)))
            return (int16_t)(buffer.uint16 - offset);
        else
            sleep_us(3200);
    }
}

bool FX29::set_scale(int _scale) {
    return true;
}

bool FX29::calibrate() {
    int32_t sum = 0;
    for (int i = 0; i < 16; i++)
        sum += get_raw_blocking();
    sum = sum >> 4;
    sum -= offset;
    this->offset = (int16_t) sum;
    return true;
}

bool FX29::start_session() {
    return true;
}

bool FX29::deinit() {
    return true;
}

FX29::~FX29() = default;

FX29::FX29() {
    this->init_done = false;
    this->scale = 1;
    this->decimal_points = 6;
    this->offset = 0;
}

std::vector<float> FX29::get_all_blocking() {
    return {get_blocking()};
}
