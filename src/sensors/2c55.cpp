//
// Created by martin on 12/19/22.
//

#include "2c55.hpp"

bool TE2c55::init(Port port) {
    init_done = false;
    this->set_scale(1);
    this->port = port;

    // TODO: FIX IT
    /* get calibration values */
    const uint8_t reg = 0x1A;
    // tell where to begin reading
    i2c_write_blocking(SENSOR_I2C, 0, &reg, 1, true);
    // read all the bytes into
    i2c_read_blocking(SENSOR_I2C, 0, reinterpret_cast<uint8_t*>(&this->calibration_data), 60, false);
    for (int i = 0; i < 30; i++) {
        printf("%#x ", reinterpret_cast<uint8_t*>(&this->calibration_data)[i*2]);
        printf("%x \n", reinterpret_cast<uint8_t*>(&this->calibration_data)[i*2+1]);
    }
    printf("Number %hd", calibration_data.min_sensor_t);
    init_done = true;
    return init_done;
}

int16_t TE2c55::get_raw_blocking() {
    // TODO reading
    return 0;
}

bool TE2c55::set_scale(int _scale) {
    return true;
}

bool TE2c55::calibrate() {
    return true;
}

bool TE2c55::start_session() {
    return true;
}

bool TE2c55::deinit() {
    return true;
}

TE2c55::~TE2c55() = default;

TE2c55::TE2c55() {
    this->init_done = false;
    this->scale = 1;
    this->decimal_points = 6;
}

std::vector<float> TE2c55::get_all_blocking() {
    return {get_blocking()};
}
