//
// Created by martin on 11/26/22.
//

#ifndef LAB_INTERFACE_KNOB_HPP
#define LAB_INTERFACE_KNOB_HPP

#include "pico/stdio.h"
#include "hardware/gpio.h"


struct knob_state {
    int rotation;
    bool press;
};

class Knob {
    bool was_pressed;
    bool was_clk;
    uint clk_pin;
    uint dt_pin;
    uint key_pin;
public:
    Knob(uint _clk_pin, uint _dt_pin, uint key_pin);
    knob_state tick();
};


#endif //LAB_INTERFACE_KNOB_HPP
