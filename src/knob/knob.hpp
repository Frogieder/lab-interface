//
// Created by martin on 11/26/22.
//

//  TODO: There's something wrong with my knob, I prolly need to take out my logic analyzer to see its weird waveform

#ifndef LAB_INTERFACE_KNOB_HPP
#define LAB_INTERFACE_KNOB_HPP

#include "pico/stdio.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define KNOB_DEBOUNCE_TIME_US 100000

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
    uint64_t next_press;
public:
    Knob(uint _clk_pin, uint _dt_pin, uint key_pin);

    knob_state tick();
};


#endif //LAB_INTERFACE_KNOB_HPP
