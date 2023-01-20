//
// Created by martin on 12/19/22.
//

#include "AD22151.hpp"

bool AD22151::init(Port port) {
    this->set_scale(1);
    this->port = port;
    this->adc_num = std::get<0>(port_mapping.at(port)) - 26;
    init_done = true;
    return init_done;
}

int16_t AD22151::get_raw_blocking() {
    adc_select_input(adc_num);
    uint16_t result = adc_read();
    return (int16_t)result;
}

bool AD22151::set_scale(int _scale) {
    if (scale < 0)
        return false;
    this->scale = 1;
    for (int i = 0; i < _scale; ++i)
        this->scale *= 10;

    this->decimal_points = 6 > _scale ? 6 - _scale : 0;
    return true;
}

bool AD22151::calibrate() {
    return true;
}

bool AD22151::start_session() {
    return true;
}

bool AD22151::deinit() {
    return true;
}

AD22151::~AD22151() = default;

AD22151::AD22151() {
    this->init_done = false;
    this->scale = 1;
    this->decimal_points = 6;
    // initialized, but yet not tu be used
    this->adc_num = 0;
}

std::vector<float> AD22151::get_all_blocking() {
    return {get_blocking()};
}

float AD22151::process_raw(uint16_t data) {
    /**
     * Bit level magic.
     * This works because the current setup has R3/R2 = 4 and ADC gives results from 0 to 4096.
     * For other R3/R2 ratios and possibly even offset, it may not be as simple.
     * The reason to use this is that Cortex M0 doesn't have an FPU.
     *
     * Current formula: (data - 2048) * 125 / 2048
     * Derived from: data * (5000mV / 4096) * (1mV / 20T)
     */
    int32_t a = (data - 2048) * 125;
    auto result = (float)a;
    // reduce the exponent of result by 11, effectively dividing it by 2048
    *(uint32_t*)&result -= 0x5800000; // =(11 << 23)
    return result;
}
