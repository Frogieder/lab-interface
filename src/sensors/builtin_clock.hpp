//
// Created by martin on 12/19/22.
//

#ifndef LAB_INTERFACE_BUILTIN_CLOCK_HPP
#define LAB_INTERFACE_BUILTIN_CLOCK_HPP

#include "isensors.hpp"

class BuiltinClock : public iSensor {
public:
    [[nodiscard]] std::string_view name() const override {return "Builtin Clock";}

    [[nodiscard]] uint n_values() const override {return 1;}

    [[nodiscard]] std::vector<Port> get_compatible_ports() const override {return {Port::NoConnection};};

    BuiltinClock();

    ~BuiltinClock() override;

    bool calibrate() override;

    bool init(Port port) override;

    bool deinit() override;

    bool set_scale(int _scale) override;

    bool start_session() override;

    int16_t get_raw_blocking() override;

    std::vector<float> get_all_blocking() override;
};



#endif //LAB_INTERFACE_BUILTIN_CLOCK_HPP
