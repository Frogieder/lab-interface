//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_ISENSORS_HPP
#define LAB_INTERFACE_ISENSORS_HPP

#define SENSOR_I2C i2c0

#include "vector"
#include "map"
#include <bits/shared_ptr.h>
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "ctime"
#include "hardware/i2c.h"
#include "hardware/adc.h"

enum class Port {
    A0, A1, A2,
    SPI0, SPI1,
    I2C0, I2C1, I2C2,
    UART0,
    None, NoConnection
};

const std::map<Port, std::string_view> port_names = {
    {Port::A0, "A0"},
    {Port::A1, "A1"},
    {Port::A2, "A2"},
    {Port::SPI0, "SPI0"},
    {Port::SPI1, "SPI1"},
    {Port::I2C0, "I2C0"},
    {Port::I2C1, "I2C1"},
    {Port::I2C2, "I2C2"},
    {Port::UART0, "UART0"},
    {Port::None, "None"},
    {Port::NoConnection, "NoConnection"}
};

enum SensorType {
    none,
    force_fx29k0,
    temp_2c55,
    temp_gy906,
    accel_ADXL312,
    hall_AD22151,
    distance_hcsr04,
    clock_builtin
};

const std::map<Port, std::tuple<uint, uint, uint, uint>> port_mapping ={
    {Port::A0, {26, 26, 26, 26}},
    {Port::A1, {27, 27, 27, 27}},
    {Port::A2, {28, 28, 28, 28}},
    /* RX, TX, SCL, CS */
    {Port::SPI0, {16, 18, 19, 17}},
    {Port::SPI1, {16, 18, 19, 22}},
    /* SDA, SCL */
    {Port::I2C0, {20, 21, 20, 21}},
    {Port::I2C1, {20, 21, 20, 21}},
    {Port::I2C2, {20, 21, 20, 21}},
    /* RX, TX */
    {Port::UART0, {9, 8, 9, 8}},
    /* Don't use the provided pins */
    {Port::NoConnection, {25, 25, 25, 25}},
    {Port::None, {25, 25, 25, 25}}
};

class iSensor {
public:
    bool init_done = false;
    int scale = 0;
    int decimal_points = 0;
    Port port = Port::None;

    [[nodiscard]] virtual std::vector<Port> get_compatible_ports() const = 0;

    /** display name of the sensor in the menu */
    [[nodiscard]] virtual std::string_view name() const = 0;

    /**  number of monitored values */
    [[nodiscard]] virtual uint n_values() const = 0;

    iSensor() = default;

    virtual ~iSensor() = default;

    /** initialize sensor (runs only once) */
    virtual bool init(Port port) = 0;

    /** turn off the sensor (runs when sensor is told to disconnect) */
    virtual bool deinit() = 0;

    /** initiate interactive calibration procedure */
    virtual bool calibrate() = 0;

    /** choose a scale on which measurements are performed
     *
     * mostly reserved for configuration over USB and for use within initialization*/
    virtual bool set_scale(int _scale) = 0;

    /** tell sensor to start collecting data */
    virtual bool start_session() = 0;

    /** wait for data on sensor and read them */
    virtual int16_t get_blocking() = 0;

    /** wait for data on sensor and read all of them as a single vector */
    virtual std::vector<int16_t> get_all_blocking() = 0;
};

#endif //LAB_INTERFACE_ISENSORS_HPP
