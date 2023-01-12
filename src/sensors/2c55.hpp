//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_2C55_HPP
#define LAB_INTERFACE_2C55_HPP

#include "isensors.hpp"

class TE2c55 : public iSensor {

    /** union used to read float data from sensor */
//    union {
//        uint8_t uValue[4];
// //       uint16_t iValue[2];
//        float fValue;
//    } sensor_reading;

    struct CalibrationData {
        int16_t min_sensor_t;
        int16_t max_senor_t;
        int16_t min_obj_t;
        int16_t max_obj_t;
        float TC;
        float T_ref;
        float k4_comp;
        float k3_comp;
        float k2_comp;
        float k1_comp;
        float k0_comp;
        float unused;
        float k4_obj;
        float k3_obj;
        float k2_obj;
        float k1_obj;
        float k0_obj;
    };
    CalibrationData calibration_data;
public:
    [[nodiscard]] std::string_view name() const override {return "2c55";}

    [[nodiscard]] std::string_view unit() const override {return "°C";}

    [[nodiscard]] uint n_values() const override {return 1;}

    [[nodiscard]] std::vector<Port> get_compatible_ports() const override {return {Port::I2C0, Port::I2C1, Port::I2C2};};

    TE2c55();

    ~TE2c55() override;

    bool calibrate() override;

    bool init(Port port) override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_raw_blocking() override;

    std::vector<float> get_all_blocking() override;
};



#endif //LAB_INTERFACE_2C55_HPP
