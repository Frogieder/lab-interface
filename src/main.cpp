#include "main.hpp"

int delay_time = 500;

[[noreturn]] void core1_entry() {
    while (true) {
        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
        sleep_ms(delay_time);
    }
}

int main() {
    stdio_init_all();
    /* I2C setup */
    i2c_init(I2C_PORT, 1000000); //Use i2c port with baud rate of 400khz
    //Set pins for I2C operation
    gpio_set_function(I2C_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_PIN_SDA);
    gpio_pull_up(I2C_PIN_SCL);

    pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(I2C_PORT, 0x3C, pico_ssd1306::Size::W128xH64);
    display.setOrientation(false);

    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    Knob knob = Knob(KNOB_CLK_PIN, KNOB_DT_PIN, KNOB_KEY_PIN);

    Menu menu = Menu(&display, &knob);

    Sensors sensors = Sensors();

    multicore_launch_core1(core1_entry);

    uint32_t start = 0;

    BuiltinClock clockkk;
    std::vector<iSensor*> listtt;
    listtt.push_back(&clockkk);
    layout_t layouttt = {};
    for (const auto& sensor : listtt)
        layouttt[420] = {sensor->name(), {0}};

    /*****************************
     *            LOOP           *
     *****************************/
    while (true) {
        auto selected = menu.menu_loop(start);
        start = 0;
        SensorType sensorType;
        switch (selected) {
            case MENU_MANAGE_ATTACH:
                sensorType = menu.choose_sensor();
                if (sensorType == SensorType::none)
                    start = MENU_MANAGE;
                else if (!sensors.add_sensor(sensorType))
                    menu.fatal_error("Sensor not implemented");
                break;
            case MENU_MANAGE_LIST:
                menu.generate_sensor_list(&sensors.list);
                menu.browse_sensors();
                break;
            default:
                menu.fatal_error("Function not implemented");
        }
    }
    return 0;
}
