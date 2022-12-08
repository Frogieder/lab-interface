//
// Created by martin on 11/28/22.
//

#ifndef LAB_INTERFACE_SENSORS_H
#define LAB_INTERFACE_SENSORS_H

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

// The indents are weird, I blame the auto-formatter
/* the vector contains pairs of titles and their corresponding submenus. The submenus are stored again as vectors,
 * however, they consist of a name and a sensor type.
 */

/** interface class for all sensor types */
class iSensor {
public:
    int scale{};

    virtual ~iSensor() = default;

    /** initialize sensor (runs only once) */
    virtual bool init() = 0;

    /** turn off the sensor (runs when sensor is told to disconnect) */
    virtual bool deinit() = 0;

    /** initiate interactive calibration procedure */
    virtual bool calibrate() = 0;

    /** choose a scale on which measurements are performed */
    virtual bool set_scale(int _scale) = 0;

    /** tell sensor to start collecting data */
    virtual bool start_session() = 0;

    /** wait for data on sensor and read them */
    virtual int16_t get_blocking() = 0;
};

class SensorClock : public iSensor {

    ~SensorClock() override;

    bool init_done = false;
    int scale = 0;

    bool calibrate() override;

    bool init() override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_blocking() override;
};

class NoSensor : public iSensor {
    bool calibrate() override { return true; }

    bool init() override { return true; }

    bool deinit() override { return true; }

    bool set_scale(int _scale) override { return true; }

    bool start_session() override { return true; }

    int16_t get_blocking() override { return 0; }
};

class Sensors {
public:
    Sensors();

    std::vector<std::shared_ptr<iSensor>> list{};

//    std::vector<iSensor*> list {};
    bool add_sensor(SensorType sensor_type);
};


#endif //LAB_INTERFACE_SENSORS_H
