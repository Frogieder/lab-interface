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
    this->next_press = 0;
}

knob_state Knob::tick() {
    bool press = false;
    int rotation = 0;

    // button state
    bool is_pressed = !gpio_get(key_pin);
    if (!was_pressed && is_pressed) {
        // check if the debounce cooldown is over
        if (next_press < time_us_64())
            press = true;
    } else if (was_pressed && !is_pressed) {
        // when button is released, start the debounce cooldown
        next_press = time_us_64() + KNOB_DEBOUNCE_TIME_US;
    }

    was_pressed = is_pressed;

    // knob state
    bool is_clk = gpio_get(clk_pin);
    if (!was_clk && is_clk)
        rotation = gpio_get(dt_pin) ? 1 : -1;
    was_clk = is_clk;

    return {rotation, press};
}
