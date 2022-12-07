//
// Created by martin on 11/15/22.
//

#include "menu.hpp"

Menu::Menu(pico_ssd1306::SSD1306 *_display, Knob *_knob) {
    display = _display;
    knob = _knob;
}

Menu::~Menu() {

}

void Menu::display_menu(uint32_t menu, uint8_t pos, layout_t * _layout) {
    if ((*_layout).contains(menu)) {
        /* Title */
        pico_ssd1306::drawText(display, font_12x16, (*_layout)[menu].first, 0, 0);

        /* Dot */
        pico_ssd1306::fillRect(display, 0, 38, 4, 42);

        /* Options */
        for (int i = 0; i <= 5; i++) {
            pico_ssd1306::drawText(display, font_8x8, this->get_item(menu, pos + (i - 2), _layout), 10, 10 * i + 16);
        }
    }
        /* Fatal error */
    else {
        this->fatal_error();
    }
    display->sendBuffer();
}


uint32_t Menu::menu_loop(uint32_t start) {
    uint32_t menu = start;
    uint8_t pos = 0;
    while (true) {
        display_menu(menu, pos, &(this->layout));
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= layout[menu].second.size())
                    pos = layout[menu].second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = layout[menu].second[pos];
                if (menu & FLAG_FUNCTION)
                    return menu;
                pos = 0;
                break;
            }
        }
    }
    return 0;
}

std::string_view Menu::get_item(uint32_t menu, uint32_t pos, layout_t * _layout) {
    if ((*_layout).contains(menu)) {
        if (pos < (*_layout)[menu].second.size()) {
            if ((*_layout).contains((*_layout)[menu].second[pos]))
                return (*_layout)[(*_layout)[menu].second[pos]].first;
            return "unnamed :((";
        }
    }
    return "";
}

[[noreturn]] void Menu::fatal_error(std::string_view text) {
    pico_ssd1306::drawText(display, font_12x16, "Error", 0, 0);
    pico_ssd1306::drawText(display, font_5x8, text, 0, 18);
    display->sendBuffer();
    while (true);
}

SensorType Menu::choose_sensor() {
    uint32_t menu = 0;
    uint8_t pos = 0;
    while (true) {
        display_menu(menu, pos, &sensor_menu_layout);
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= sensor_menu_layout[menu].second.size())
                    pos = sensor_menu_layout[menu].second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = sensor_menu_layout[menu].second[pos];
                if (menu > 0xf) {
                    if (sensor_layout_enum_map.contains(menu))
                        return sensor_layout_enum_map[menu];
                    else
                        fatal_error("Wrong sensor: " + std::to_string(menu));
                }
                pos = 0;
                break;
            }
        }

    }
    return temp_gy906;
}
