//
// Created by martin on 11/28/22.
//

#include "sensors.hpp"

/* CLOCK */

bool Sensors::add_sensor(SensorType sensor_type, bool init) {
    switch (sensor_type) {
        case SensorType::clock_builtin:
            list.push_back(std::make_shared<BuiltinClock>());
            if (init)
                list.at(list.size() - 1)->init();
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

Sensors::Sensors()
    : connections() {
    list.reserve(8);
}
