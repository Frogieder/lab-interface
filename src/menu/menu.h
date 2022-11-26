//
// Created by martin on 11/15/22.
//

#ifndef LAB_INTERFACE_MENU_H
#define LAB_INTERFACE_MENU_H

#include <map>
#include <vector>
#include <string_view>
#include "pico/stdlib.h"
#include "../../pico-ssd1306/ssd1306.h"
#include "../../pico-ssd1306/shapeRenderer/ShapeRenderer.h"
#include "../../pico-ssd1306/textRenderer/TextRenderer.h"

#define ITEM_IS_FUNCTION 0x80000000

#define MENU_ROOT 0

#define MENU_ATTACH 0x1
#define MENU_ATTACH_SCAN 0x11
#define MENU_ATTACH_ADD 0x12

#define MENU_MONITOR 0x2
#define MENU_START 0x3
#define MENU_CALIBRATE 0x4
#define MENU_DUMMY_ONE 0x5
#define MENU_DUMMY_TWO 0x6

class Menu {

    /*
     * map representing a tree structure of the menu.
     * Submenus are stored in a vector, where each submenu is represented by a pair, containing its index and name
     * Each submenu has its index, where last four bits are unique to the menu item, the previous bits are the index of its parent (its path).
     */
    std::map<uint32_t, std::pair<const char*, std::vector<uint32_t>>> layout;
    pico_ssd1306::SSD1306 *display;
public:
    explicit Menu(pico_ssd1306::SSD1306 * _display);
    ~Menu();
    uint32_t display_menu(uint32_t menu, uint8_t pos=0);
    uint32_t menu_loop(uint32_t start=MENU_ROOT);
    std::string_view get_item(uint32_t menu, uint32_t pos);
};


#endif //LAB_INTERFACE_MENU_H
