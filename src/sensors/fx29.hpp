//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_FX29_HPP
#define LAB_INTERFACE_FX29_HPP

#include "isensors.hpp"

class FX29 : public iSensor {
    int16_t offset;
public:
    [[nodiscard]] std::string_view name() const override {return "fx29";}

    [[nodiscard]] std::string_view unit() const override {return "N";}

    [[nodiscard]] uint n_values() const override {return 1;}

    [[nodiscard]] std::vector<Port> get_compatible_ports() const override {return {Port::I2C0, Port::I2C1, Port::I2C2};};

    FX29();

    ~FX29() override;

    bool calibrate() override;

    bool init(Port port) override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_raw_blocking() override;

    std::vector<float> get_all_blocking() override;
};



#endif //LAB_INTERFACE_FX29_HPP
