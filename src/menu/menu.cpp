//
// Created by martin on 11/15/22.
//

#include "menu.hpp"

Menu::Menu(pico_ssd1306::SSD1306 * _display,  Knob * _knob) {
    display = _display;
    knob = _knob;

    layout = {
            {MENU_ROOT, {"Menu", {MENU_ATTACH, MENU_MONITOR, MENU_START, MENU_CALIBRATE, MENU_DUMMY_ONE, MENU_DUMMY_TWO}}},
            /* Attaching submenu */
            {MENU_ATTACH, {"Attach", {MENU_ROOT ,MENU_ATTACH_SCAN, MENU_ATTACH_ADD}}},
            {MENU_ATTACH_ADD, {"Add sensor", {MENU_ATTACH}}},
            {MENU_ATTACH_SCAN, {"Scan", {MENU_ATTACH}}},
            /* Monitoring submenu */
            {MENU_MONITOR, {"Monitor", {MENU_ROOT}}},
            /* Measurement submenu */
            {MENU_START, {"Measure", {MENU_ROOT}}},
            /* Calibration menu */
            {MENU_CALIBRATE, {"Calibrate", {MENU_ROOT}}},
            /* Dummies */
            {MENU_DUMMY_ONE, {"Dummy 1", {MENU_ROOT}}},
            {MENU_DUMMY_TWO, {"Dummy 2", {MENU_ROOT}}},
    };
}

Menu::~Menu() {

}

uint32_t Menu::display_menu(uint32_t menu, uint8_t pos) {
    if (layout.contains(menu)) {
        /* Title */
        pico_ssd1306::drawText(display, font_12x16, this->layout[menu].first, 0, 0);

        /* Dot */
        pico_ssd1306::fillRect(display, 0, 38, 4, 42);

        /* Options */
        for (int i = 0; i <= 5; i++) {
            pico_ssd1306::drawText(display, font_8x8, this->get_item(menu, pos+(i-2)), 10, 10*i + 16);
        }
    }
    /* Fatal error */
    else {
        pico_ssd1306::drawText(display, font_12x16, "Error", 0, 0);
    }
    display->sendBuffer();
    return 0;
}

uint32_t Menu::menu_loop(uint32_t start) {
    uint32_t menu = start;
    uint8_t pos = 0;
    while (true) {
        display_menu(menu, pos);
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
                pos = 0;
                break;
            }
        }

    }
    return 0;
}

std::string_view Menu::get_item(uint32_t menu, uint32_t pos) {
    if (layout.contains(menu)) {
        if (pos < layout[menu].second.size()) {
            if (layout.contains(layout[menu].second[pos]))
                return layout[layout[menu].second[pos]].first;
            return "unnamed :((";
        }
    }
    return "";
}
