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

Sensors::Sensors() {
    // ADC initialization is done here only once,
    // since it doesn't make sense to have it done individually for each analog sensor
    adc_init();

    // reserve capacity now to avoid future reallocations, as there will never be more than 11 connected sensors
    list.reserve(11);
}

ConnectionState Sensors::get_state(Port port) {
    if (this->connections.contains(port))
        return connections[port];
    else return ConnectionState::Available;
}

void Sensors::set_state(Port port, ConnectionState state) {
    connections[port] = state;
}
