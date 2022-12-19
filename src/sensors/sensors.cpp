//
// Created by martin on 11/28/22.
//

#include "sensors.hpp"

#include <memory>


/* CLOCK */
bool SensorClock::init() {
    init_done = true;
    return init_done;
}

int16_t SensorClock::get_blocking() {
    return (int16_t) ((time_us_32() / scale) & 0xffff);
}

bool SensorClock::set_scale(int _scale) {
    this->scale = _scale;
    return true;
}

bool SensorClock::calibrate() {
    return true;
}

bool SensorClock::start_session() {
    return true;
}

bool SensorClock::deinit() {
    return true;
}

SensorClock::~SensorClock() = default;

SensorClock::SensorClock() {
    this->init_done = false;
    this->scale = 0;
};

bool Sensors::add_sensor(SensorType sensor_type) {
//    list.push_back(std::shared_ptr<NoSensor>());
    switch (sensor_type) {
        case SensorType::clock_builtin:
            list.push_back(std::make_shared<SensorClock>());
//            list.push_back(new SensorClock);
            return true;
//            break;
        case force_fx29k0:
//            break;
        case temp_2c55:
//            break;
        case temp_gy906:
//            break;
        case accel_ADXL312:
//            break;
        case hall_AD22151:
//            break;
        case distance_hcsr04:
            break;
        case none:
            return true;
            break;
    }
    return false;
}

Sensors::Sensors() {
    list.reserve(8);
//    for (int i = 0; i < 8; i++)
//        list.push_back(std::shared_ptr<NoSensor>());
}
