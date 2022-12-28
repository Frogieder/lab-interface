//
// Created by martin on 11/12/22.
//

#ifndef TEST_PROJECT_MAIN_H
#define TEST_PROJECT_MAIN_H

#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/multicore.h"
#include <map>
#include <string>
#include "pico/time.h"
#include <stdio.h>

#include "../pico-ssd1306/ssd1306.h"
#include "../pico-ssd1306/shapeRenderer/ShapeRenderer.h"
#include "../pico-ssd1306/textRenderer/TextRenderer.h"

#include "misc/miscellaneous_functions.hpp"
#include "menu/menu.hpp"
#include "knob/knob.hpp"
#include "sensors/sensors.hpp"

#define I2C_PORT i2c1

#define I2C_PIN_SDA 2
#define I2C_PIN_SCL 3

#define KNOB_CLK_PIN 4
#define KNOB_DT_PIN 5
#define KNOB_KEY_PIN 6

#endif //TEST_PROJECT_MAIN_H
