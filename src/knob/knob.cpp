//
// Created by martin on 11/26/22.
//

#include "knob.hpp"

Knob::Knob(uint _clk_pin, uint _dt_pin, uint _key_pin) {
    this->clk_pin = _clk_pin;
    this->dt_pin = _dt_pin;
    this->key_pin = _key_pin;
    gpio_init(clk_pin);
    gpio_set_dir(clk_pin, GPIO_IN);
    gpio_init(dt_pin);
    gpio_set_dir(dt_pin, GPIO_IN);
    gpio_init(key_pin);
    gpio_set_dir(key_pin, GPIO_IN);
}

knob_state Knob::tick() {
    bool press = false;
    int rotation = 0;

    // button state
    bool is_pressed = gpio_get(key_pin);
    if (was_pressed && !is_pressed)
        press = true;

    was_pressed = is_pressed;

    // knob state
    bool is_clk = gpio_get(clk_pin);
    if (!was_clk && is_clk)
        rotation =  gpio_get(dt_pin) ? 1 : -1;
    was_clk = is_clk;

    return {rotation, press};
}
