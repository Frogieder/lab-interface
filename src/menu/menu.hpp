//
// Created by martin on 11/15/22.
//

#ifndef LAB_INTERFACE_MENU_H
#define LAB_INTERFACE_MENU_H

#include <map>
#include <vector>
#include <string_view>
#include "ranges"
#include "../sensors/sensors.hpp"
#include "pico/stdlib.h"
#include "../knob/knob.hpp"
#include "../../pico-ssd1306/ssd1306.h"
#include "../../pico-ssd1306/shapeRenderer/ShapeRenderer.h"
#include "../../pico-ssd1306/textRenderer/TextRenderer.h"

#define FLAG_FUNCTION 0x80000000
#define FLAG_SENSOR 0x40000000
#define FLAG_CANCEL 0x20000000

#define MENU_ROOT 0

#define MENU_MANAGE 0x1
#define MENU_MANAGE_LIST (0x10 | FLAG_FUNCTION)
#define MENU_MANAGE_ATTACH (0x11 | FLAG_FUNCTION)

#define MENU_MONITOR 0x2
#define MENU_START 0x3
#define MENU_CALIBRATE 0x4
#define MENU_DUMMY_ONE 0x5
#define MENU_DUMMY_TWO 0x6

#define MENU_CANCEL (FLAG_CANCEL | 1)

#define MENU_SENSORS_MONITOR (0x101 | FLAG_FUNCTION)
#define MENU_SENSORS_REMOVE (0x102 | FLAG_FUNCTION)

#define SENSORS_MENU (0 | FLAG_SENSOR)
#define SENSOR_CLOCK (1 | FLAG_SENSOR)

typedef std::map<uint32_t, std::pair<std::string_view, std::vector<uint32_t>>> layout_t;


class Menu {

    /**
     * map representing a tree structure of the menu.
     * Submenus are stored in a vector, where each submenu is represented by a pair, containing its index and name
     * Each submenu has its index, where last four bits are unique to the menu item, the previous bits are the index of its parent (its path).
     */
    pico_ssd1306::SSD1306 *display;
    Knob *knob;
    Sensors *sensors;
    layout_t main_menu_layout = {
        {MENU_ROOT,          {"Main Menu",     {MENU_MANAGE, MENU_MONITOR,     MENU_START, MENU_CALIBRATE,
                                                   MENU_DUMMY_ONE, MENU_DUMMY_TWO}}},
        /* Management submenu */
        {MENU_MANAGE,        {"Manage",        {MENU_ROOT,   MENU_MANAGE_LIST, MENU_MANAGE_ATTACH}}},
        {MENU_MANAGE_ATTACH, {"Attach",        {}}},
        {MENU_MANAGE_LIST,   {"List sensors",  {}}},
        /* Monitoring submenu */
        {MENU_MONITOR,       {"Monitor",       {MENU_ROOT}}},
        /* Measurement submenu */
        {MENU_START,         {"Measure",       {MENU_ROOT}}},
        /* Calibration menu */
        {MENU_CALIBRATE,     {"Calibrate",     {MENU_ROOT}}},
        /* Dummies */
        {MENU_DUMMY_ONE,     {"Dummy 1",       {MENU_ROOT}}},
        {MENU_DUMMY_TWO,     {"Dummy 2",       {MENU_ROOT}}},

        // I forgot why I need it, but without this I can't get the sensor selection to work
        // TODO: Find why it's needed and then safely remove it
        /* SENSORS */
        {SENSORS_MENU,       {"Select sensor", {MENU_ROOT,   SENSOR_CLOCK}}},

        /* SPECIAL */
        {MENU_CANCEL,        {"Cancel",        {MENU_ROOT}}}
    };


    layout_t sensor_selection_layout = {
        {MENU_ROOT,   {"All sensors",   {MENU_CANCEL, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6}}},
        {MENU_CANCEL, {"Back to menu",  {MENU_ROOT}}},
        /* Force */
        {0x1,         {"Force sensor",  {MENU_ROOT,   0x10}}},
        {0x10,        {"TE: FX29K0",    {}}},
        /* Temperature */
        {0x2,         {"Temperature",   {MENU_ROOT,   0x20}}},
        {0x20,        {"GY906",         {}}},
        {0x21,        {"2c55",          {}}},
        /* Acceleration */
        {0x3,         {"Acceleration",  {MENU_ROOT,   0x30}}},
        {0x30,        {"ADXL312",       {}}},
        /* Distance */
        {0x4,         {"Distance",      {MENU_ROOT,   0x40}}},
        {0x40,        {"HC-SR04",       {}}},
        /* Hall effect */
        {0x5,         {"Hall",          {MENU_ROOT,   0x50}}},
        {0x50,        {"AD22151",       {}}},
        /* Timers */
        {0x6,         {"Clock",         {MENU_ROOT,   0x60}}},
        {0x60,        {"Builtin clock", {}}}
    };


    std::map<uint32_t, SensorType> sensor_layout_enum_map = {
        {0x00,        SensorType::none},
        {0x10,        SensorType::force_fx29k0},
        {0x20,        SensorType::temp_gy906},
        {0x20,        SensorType::temp_2c55},
        {0x30,        SensorType::accel_ADXL312},
        {0x40,        SensorType::distance_hcsr04},
        {0x50,        SensorType::hall_AD22151},
        {0x60,        SensorType::clock_builtin},
        {MENU_CANCEL, SensorType::none}
    };

    layout_t connected_sensor_list_layout = {
        {MENU_ROOT,   {"Select sensor", {MENU_CANCEL}}},
        {MENU_CANCEL, {"Back to menu",  {}}},
        {MENU_SENSORS_REMOVE, {"Detach", {}}},
        {MENU_SENSORS_MONITOR, {"Monitor", {}}}
    };

public:
    explicit Menu(pico_ssd1306::SSD1306 *_display, Knob *_knob, Sensors *_sensors);

    ~Menu();

    /** displays menu onto the display given menu index and position of the selected item */
    void display_menu(uint32_t menu, uint8_t pos, layout_t *_layout);

    /** loop to get command from user using interactive submenus */
    uint32_t menu_loop(uint32_t start = MENU_ROOT);

    /** returns the name of a menu given submenu and a position to look into */
    std::string_view get_item(uint32_t menu, uint32_t pos, layout_t *_layout);

    /** prompts the user to choose a sensor from the list */
    SensorType choose_sensor();

    /** prompts the user to choose a port for a sensor */
    Port choose_port(std::unique_ptr<layout_t> layout, const std::vector<Port>& ports);

    void monitor_sensor(uint32_t sensor);

    /** call this in case everything fails and cry */
    [[noreturn]] void fatal_error(std::string_view text = "");

    /** Update the contents of the sensor list */
    void generate_sensor_list(sensorlist_t *sensorlist);

    std::unique_ptr<layout_t> generate_port_list_layout(const std::vector<Port>& ports);

    /** Browse and manage attached sensors */
    void browse_sensors();

    /** Return the font that can fit the text of given length */
    static unsigned char *choose_font(uint length);
};


#endif //LAB_INTERFACE_MENU_H
