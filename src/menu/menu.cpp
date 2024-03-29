//
// Created by martin on 11/15/22.
//

#include "menu.hpp"

Menu::Menu(pico_ssd1306::SSD1306 *_display, Knob *_knob, Sensors *_sensors) {
    display = _display;
    knob = _knob;
    sensors = _sensors;
}

Menu::~Menu() = default;

void Menu::display_menu(uint32_t menu, uint8_t pos, layout_t *_layout) {
    display->clear();
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
        this->fatal_error("display_menu", "menu.c");
    }
    display->sendBuffer();
}


uint32_t Menu::menu_loop(uint32_t start) {
    uint32_t menu = start;
    uint8_t pos = 0;
    while (true) {
        display_menu(menu, pos, &(this->main_menu_layout));

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

[[noreturn]] void Menu::fatal_error(std::string_view text, std::string_view text1, std::string_view text2) {
    display->clear();
    pico_ssd1306::drawText(display, font_12x16, "Error", 0, 0);
    pico_ssd1306::drawText(display, font_5x8, text, 0, 18);
    pico_ssd1306::drawText(display, font_5x8, text1, 0, 28);
    pico_ssd1306::drawText(display, font_5x8, text2, 0, 38);
    display->sendBuffer();
    while (true);
}

void Menu::generate_sensor_list(sensorlist_t *sensorlist) {
    // Remove all the sensors from the list
    for (auto& elem : this->connected_sensor_list_layout | std::views::reverse) {
        if (elem.first & FLAG_SENSOR)
            this->connected_sensor_list_layout.erase(elem.first);
    }
    // Remove links to sensors from MENU_ROOT
    this->connected_sensor_list_layout[MENU_ROOT].second.erase(connected_sensor_list_layout[MENU_ROOT].second.begin() + 1, connected_sensor_list_layout[MENU_ROOT].second.end());

    // Add current sensors to the list
    int i = FLAG_SENSOR;
    for (const auto& sensor : *sensorlist){
        this->connected_sensor_list_layout[i] = {sensor->name(), {MENU_ROOT, MENU_SENSORS_MONITOR, MENU_SENSORS_REMOVE}};
        this-> connected_sensor_list_layout[MENU_ROOT].second.push_back(i);
        i++;
    }
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
                        fatal_error("Wrong sensor: " + std::to_string(menu), "menu.cpp");
                }
                pos = 0;
                break;
            }
        }

    }
    return SensorType::none;
}

void Menu::browse_sensors() {
    uint32_t menu = 0;
    uint8_t pos = 0;
    uint32_t current_sensor = 0;
    while (true) {
        display->clear();
        display_menu(menu, pos, &connected_sensor_list_layout);

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
                            this->monitor_sensor(current_sensor & (~FLAG_SENSOR));
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

void Menu::monitor_sensor(uint32_t sensor) {
    auto font = choose_font((*sensors).list[sensor]->name().length());
    while (true) {
        auto next_update = time_us_64() + 300000;
        while(time_us_64() < next_update) {
            knob_state state = knob->tick();
            if (state.press) {
                return;
            }
        }
        display->clear();
        pico_ssd1306::drawText(display, font, (*sensors).list[sensor]->name(), 0, 0);
        float val = (*sensors).list[sensor]->process_raw((*sensors).list[sensor]->get_raw_average_blocking());
        pico_ssd1306::drawText(display, font_8x8,float_to_string(val) + " " + std::string((*sensors).list[sensor]->unit()),0, 20);
//        pico_ssd1306::drawText(display, font_8x8,
//                               std::bitset<16>((*sensors).list[sensor]->get_raw_blocking()).to_string(),0, 20);
        display->sendBuffer();
    }
}

unsigned char *Menu::choose_font(uint length) {
    if (length <= 10) {
        return (unsigned char *) font_12x16;
    }
    else if (length <= 16) {
        return (unsigned char *) font_8x8;
    }
    return (unsigned char *) font_5x8;
}

std::unique_ptr<layout_t> Menu::generate_port_list_layout(const std::vector<Port>& ports) { // NOLINT(readability-convert-member-functions-to-static)
    auto result = std::make_unique<layout_t>();
    (*result)[MENU_ROOT] = {"Choose port", {MENU_CANCEL}};
    (*result)[MENU_CANCEL] = {"Cancel", {}};
    for (int i = 0; i < ports.size(); ++i) {
        if (sensors->get_state(ports[i]) != ConnectionState::Available)
            continue;
        (*result)[i | FLAG_FUNCTION] = {port_names.at(ports[i]), {}};
        (*result)[MENU_ROOT].second.push_back(i | FLAG_FUNCTION);
    }
    return result;
}

Port Menu::choose_port(std::unique_ptr<layout_t> layout, const std::vector<Port>& ports) {
    uint32_t menu = 0;
    uint8_t pos = 0;
    while (true) {
        display_menu(menu, pos, layout.get());
        display->clear();

        /* wait for user input */
        while (true) {
            /* knob rotation */
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= (*layout).at(menu).second.size())
                    pos = (*layout).at(menu).second.size() - 1;
                break;
            }
            /* knob pressing */
            if (state.press) {
                menu = (*layout)[menu].second[pos];
                if (menu == MENU_CANCEL)
                    return Port::None;
                return ports[menu & (~FLAG_FUNCTION)];
                break;
            }
        }
    }
}

void Menu::measure() {
    /* Confirm the user is ready */
    uint32_t menu = 0;
    uint8_t pos = 0;
    bool ready = false;
    while (!ready) {
        display_menu(menu, pos, &pre_measurement_layout);
        display->clear();
        /* Wait for user input */
        while (true) {
            knob_state state = this->knob->tick();
            if (state.rotation != 0) {
                pos += state.rotation;
                if (pos == 0xff)
                    pos = 0;
                else if (pos >= pre_measurement_layout[menu].second.size())
                    pos = pre_measurement_layout[menu].second.size() - 1;
                break;
            }
            if (state.press) {
                menu = pre_measurement_layout[menu].second[pos];
                if (menu == MENU_CANCEL)
                    return;
                if (menu == MENU_MEASURE) {
                    ready = true;
                    break;
                }
            }
        }
    }

    drawText(display, font_12x16, "Measuring...", 0, 0);
    display->sendBuffer();
    display->clear();

    /* Inform about available sensors */
    printf("\n!STARTING MEASUREMENT#\n");
    for (const auto& sensor : sensors->list) {
        auto sub = sensor->name().substr(0, sensor->name().length());
        printf("=%.*s=", static_cast<int>(sub.length()), sub.data());
    }
    printf("\n");

    /* Measurement loop */
    auto state = knob->tick();
    auto next_update = time_us_64();
    while (!state.press) {
        // Wait for next send
        // TODO: variable frequency
        do {
            state = knob->tick();
            if (state.press) {
                printf("!END#\n");
                return;
            }
        } while(time_us_64() < next_update);
        next_update = time_us_64() + 1000;

        // Measure each sensor
        for (auto sensor : sensors->list) {
            printf("%04x,", sensor->get_raw_average_blocking());
        }
        printf("\n");
    }
}

std::string float_to_string(float val) {
    const int n = __gnu_cxx::__numeric_traits<float>::__max_exponent10 + 20;
    return __gnu_cxx::__to_xstring<std::string>(&std::vsnprintf, n,"% .2f", val);
}
