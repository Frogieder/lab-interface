//
// Created by martin on 11/28/22.
//

#include "sensors.hpp"

/* CLOCK */

std::shared_ptr<iSensor> Sensors::add_sensor(SensorType sensor_type) {
    std::shared_ptr<iSensor> sensor;
    switch (sensor_type) {
        case SensorType::clock_builtin:
            sensor = std::make_shared<BuiltinClock>();
            list.push_back(sensor);
            break;
        case hall_AD22151:
            sensor = std::make_shared<AD22151>();
            list.push_back(sensor);
            break;
        case force_fx29k0:
//            break;
        case temp_2c55:
//            break;
        case temp_gy906:
//            break;
        case accel_ADXL312:
//            break;
        case distance_hcsr04:
//            break;
        case none:
            sensor = nullptr;
            break;
    }
    return sensor;
}

Sensors::Sensors() {
    /* Port initialization is done here only once,
     * since it doesn't make sense to have it done individually for each sensor */

    // ADC init
    adc_init();
    adc_gpio_init(std::get<0>(port_mapping.at(Port::A0)));
    adc_gpio_init(std::get<0>(port_mapping.at(Port::A1)));
    adc_gpio_init(std::get<0>(port_mapping.at(Port::A2)));

    // I2C init
    i2c_init(SENSOR_I2C, 1000000);
    gpio_set_function(std::get<0>(port_mapping.at(Port::I2C0)), GPIO_FUNC_I2C);
    gpio_set_function(std::get<1>(port_mapping.at(Port::I2C0)), GPIO_FUNC_I2C);

    // TODO: SPI and UART init

    // Initialize all ports as available
    connections = {
        {Port::A0, ConnectionState::Available},
        {Port::A1, ConnectionState::Available},
        {Port::A2, ConnectionState::Available},
        {Port::I2C0, ConnectionState::Available},
        {Port::I2C1, ConnectionState::Available},
        {Port::I2C2, ConnectionState::Available},
        {Port::SPI0, ConnectionState::Available},
        {Port::SPI1, ConnectionState::Available},
        {Port::UART0, ConnectionState::Available},
        {Port::NoConnection, ConnectionState::Available},
    };

    /* reserve capacity now to avoid future reallocations, as there will never be more than 11 connected sensors */
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
