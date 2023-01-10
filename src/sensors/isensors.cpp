//
// Created by martin on 1/11/23.
//

#include "isensors.hpp"

uint16_t iSensor::get_raw_average_blocking(uint n) {
    uint32_t sum = 0;
    for (int i = 0; i < (1<<n); ++i)
        sum += get_raw_blocking();
    sum = sum >> n;
    return (uint16_t)sum;
}
