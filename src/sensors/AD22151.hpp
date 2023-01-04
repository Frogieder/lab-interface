//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_AD22151_HPP
#define LAB_INTERFACE_AD22151_HPP

#include "isensors.hpp"

class AD22151 : public iSensor {
public:
    [[nodiscard]] std::string_view name() const override {return "Builtin Clock";}

    [[nodiscard]] uint n_values() const override {return 1;}

    AD22151();

    ~AD22151() override;

    bool calibrate() override;

    bool init() override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_blocking() override;

    std::vector<int16_t> get_all_blocking() override;
};



#endif //LAB_INTERFACE_AD22151_HPP
