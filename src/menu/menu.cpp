//
// Created by martin on 11/15/22.
//

#include "menu.hpp"

Menu::Menu(pico_ssd1306::SSD1306 *_display, Knob *_knob) {
    display = _display;
    knob = _knob;
}

Menu::~Menu() = default;

void Menu::display_menu(uint32_t menu, uint8_t pos, layout_t *_layout) {
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
        display_menu(menu, pos, &(this->main_menu_layout));
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= main_menu_layout[menu].second.size())
                    pos = main_menu_layout[menu].second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = main_menu_layout[menu].second[pos];
                if (menu & FLAG_FUNCTION)
                    return menu;
                pos = 0;
                break;
            }
        }
    }
    return 0;
}

std::string_view Menu::get_item(uint32_t menu, uint32_t pos, layout_t *_layout) {
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
        display_menu(menu, pos, &sensor_selection_layout);
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= sensor_selection_layout[menu].second.size())
                    pos = sensor_selection_layout[menu].second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = sensor_selection_layout[menu].second[pos];
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
    return SensorType::none;
}

void Menu::generate_sensor_list(sensorlist_t *sensorlist) {
    for (auto& elem : this->connected_sensor_list_layout | std::views::reverse) {
        if (elem.first & FLAG_SENSOR)
            this->connected_sensor_list_layout.erase(elem.first);
    }
    this->connected_sensor_list_layout[MENU_ROOT].second.erase(connected_sensor_list_layout[MENU_ROOT].second.begin() + 1, connected_sensor_list_layout[MENU_ROOT].second.end());

    int i = FLAG_SENSOR;
    for (const auto& sensor : *sensorlist){
        this->connected_sensor_list_layout[i] = {sensor->name(), {MENU_ROOT, MENU_SENSORS_MONITOR, MENU_SENSORS_REMOVE}};
        this-> connected_sensor_list_layout[MENU_ROOT].second.push_back(i);
        i++;
    }
}

void Menu::browse_sensors() {
    uint32_t menu = 0;
    uint8_t pos = 0;
    uint32_t current_sensor = 0;
    while (true) {
        display_menu(menu, pos, &connected_sensor_list_layout);
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= connected_sensor_list_layout[menu].second.size())
                    pos = connected_sensor_list_layout[menu].second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = connected_sensor_list_layout[menu].second[pos];
                if (menu == MENU_CANCEL)
                    return;
                if (menu & FLAG_SENSOR) {
                    current_sensor = menu;
                    break;
                }
                if (menu & FLAG_FUNCTION) {
                    switch (menu) {
                        case MENU_SENSORS_MONITOR:

                            menu = current_sensor;
                            break;
                        case MENU_SENSORS_REMOVE:
                        default:
                            this->fatal_error("sensor function " + std::to_string(menu));
                    }
                }
                pos = 0;
                break;
            }
        }
    }
}

void Menu::monitor_sensor(uint32_t) {

}
