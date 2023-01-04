//
// Created by martin on 11/28/22.
//

#ifndef LAB_INTERFACE_SENSORS_H
#define LAB_INTERFACE_SENSORS_H

#include "isensors.hpp"

// Include all supported sensors
#include "builtin_clock.hpp"
#include "AD22151.hpp"

/** interface class for all sensor types */
typedef std::vector<std::shared_ptr<iSensor>> sensorlist_t;


// /* Placeholder class */
// /* Left here just in case it may be needed later */
//class NoSensor : public iSensor {
//    bool calibrate() override { return true; }
//
//    bool init() override { return true; }
//
//    bool deinit() override { return true; }
//
//    bool set_scale(int _scale) override { return true; }
//
//    bool start_session() override { return true; }
//
//    int16_t get_blocking() override { return 0; }
//};

// TODO: change my other enums to enum classes

/** Enum defining possible connection states of every port on the device */
enum class ConnectionState {
    Available,
    Connected,
    /** Port occupied by neighbouring port
     *
     * For example, HCSR-04 is connected to I2C1, but doesn't use I2C protocol.
     * This renders I2C0 and I2C2 unusable, because they rely on multiplexing connections, which is only possible
     * with regular I2C.
     */
    Unavailable,
    Error
};

/** map of all connections */
struct Connections {
    ConnectionState A0 = ConnectionState::Available;
    ConnectionState A1 = ConnectionState::Available;
    ConnectionState A2 = ConnectionState::Available;
    ConnectionState SPI0 = ConnectionState::Available;
    ConnectionState SPI1 = ConnectionState::Available;
    ConnectionState I2C0 = ConnectionState::Available;
    ConnectionState I2C1 = ConnectionState::Available;
    ConnectionState I2C2 = ConnectionState::Available;
    ConnectionState UART0 = ConnectionState::Available;
};

/** container for all connected sensors */
struct Sensors {
    Sensors();

    std::vector<std::shared_ptr<iSensor>> list{};

    Connections connections;

    bool add_sensor(SensorType sensor_type, bool init=true);
};

#endif //LAB_INTERFACE_SENSORS_H
