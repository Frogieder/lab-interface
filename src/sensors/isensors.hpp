//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_ISENSORS_HPP
#define LAB_INTERFACE_ISENSORS_HPP

#include "vector"
//#include "memory"
#include <bits/shared_ptr.h>
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "ctime"
#include "hardware/i2c.h"



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

class iSensor {
public:
    bool init_done = false;
    int scale = 0;
    int decimal_points = 0;

    /** display name of the sensor in the menu */
    [[nodiscard]] virtual std::string_view name() const = 0;

    /**  number of monitored values */
    [[nodiscard]] virtual uint n_values() const = 0;

    iSensor() = default;

    virtual ~iSensor() = default;

    /** initialize sensor (runs only once) */
    virtual bool init() = 0;

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
};

#endif //LAB_INTERFACE_ISENSORS_HPP
