//
// Created by martin on 11/15/22.
//

#include "menu.h"

Menu::Menu(pico_ssd1306::SSD1306 * _display) {
    display = _display;
    layout = {
            {MENU_ROOT, {"Menu", {MENU_ATTACH, MENU_MONITOR, MENU_START, MENU_CALIBRATE, }}},
            /* Attaching submenu */
            {MENU_ATTACH, {"Attach", {MENU_ROOT ,MENU_ATTACH_SCAN, MENU_ATTACH_ADD}}},
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
        pico_ssd1306::drawText(display, font_12x16, this->layout[menu].first, 10, 0);
        for (int i = 0; i <= 5; i++) {
            pico_ssd1306::drawText(display, font_8x8, this->get_item(menu, pos+i), 10, 10*i + 16);
        }
    }
    else {
        pico_ssd1306::drawText(display, font_12x16, "Error", 0, 0);
    }
    display->sendBuffer();
    return 0;
}

uint32_t Menu::menu_loop(uint32_t start) {
    display_menu(start, 0);
    display->clear();
    return 0;
}

std::string_view Menu::get_item(uint32_t menu, uint32_t pos) {
    if (layout.contains(menu)) {
        if (pos < layout[menu].second.size()) {
            if (layout.contains(layout[menu].second[pos]))
                return layout[layout[menu].second[pos]].first;
        }
    }
    return "";
}
