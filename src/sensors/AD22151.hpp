//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_AD22151_HPP
#define LAB_INTERFACE_AD22151_HPP

#include "isensors.hpp"

class AD22151 : public iSensor {
    uint adc_num;
public:
    [[nodiscard]] std::string_view name() const override {return "AD22151";}

    [[nodiscard]] std::string_view unit() const override {return "m/s^2";}

    [[nodiscard]] uint n_values() const override {return 1;}

    [[nodiscard]] std::vector<Port> get_compatible_ports() const override {return {Port::A0, Port::A1, Port::A2};};

    AD22151();

    ~AD22151() override;

    bool calibrate() override;

    bool init(Port port) override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_raw_blocking() override;

    std::vector<float> get_all_blocking() override;
};



#endif //LAB_INTERFACE_AD22151_HPP
