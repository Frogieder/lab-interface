#include "main.hpp"

int delay_time = 500;

void core1_entry() {
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
    /*****************************
     *            LOOP           *
     *****************************/
    while(true) {
        delay_time = 500;
        auto selected = menu.menu_loop();
        SensorType sensorType;
        switch (selected) {
            case MENU_ATTACH:
                delay_time = 100;
                sensorType = menu.choose_sensor();
                delay_time = 300;
                if (!sensors.add_sensor(sensorType))
                    menu.fatal_error("Sensor not implemented");
                break;
            default:
                menu.fatal_error("Function not implemented");
        }
    }
    return 0;
}
