//
// Created by martin on 12/19/22.
//

#include "builtin_clock.hpp"

bool BuiltinClock::init(Port port) {
    this->set_scale(5);
    init_done = true;
    return init_done;
}

int16_t BuiltinClock::get_raw_blocking() {
    return (int16_t) ((time_us_32() / scale) & 0xffff);
}

bool BuiltinClock::set_scale(int _scale) {
    if (scale < 0)
        return false;
    this->scale = 1;
    for (int i = 0; i < _scale; ++i)
        this->scale *= 10;

    this->decimal_points = 6 > _scale ? 6 - _scale : 0;
    return true;
}

bool BuiltinClock::calibrate() {
    return true;
}

bool BuiltinClock::start_session() {
    return true;
}

bool BuiltinClock::deinit() {
    return true;
}

BuiltinClock::~BuiltinClock() = default;

BuiltinClock::BuiltinClock() {
    this->init_done = false;
    this->scale = 1;
    this->decimal_points = 6;
}

std::vector<float> BuiltinClock::get_all_blocking() {
    return {get_blocking()};
}
