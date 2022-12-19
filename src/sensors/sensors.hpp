//
// Created by martin on 11/28/22.
//

#ifndef LAB_INTERFACE_SENSORS_H
#define LAB_INTERFACE_SENSORS_H

#include "isensors.hpp"
#include "builtin_clock.hpp"

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

/** container for all connected sensors */
class Sensors {
public:
    Sensors();

    std::vector<std::shared_ptr<iSensor>> list{};

//    std::vector<iSensor*> list {};
    bool add_sensor(SensorType sensor_type, bool init=true);
};

#endif //LAB_INTERFACE_SENSORS_H
