#include "main.hpp"

void core1_entry() {
    while (true) {
        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
        sleep_ms(300);
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

    multicore_launch_core1(core1_entry);
    int i = 0;
    /*****************************
     *            LOOP           *
     *****************************/
//    while (true) {
//        display.clear();
//        if (knob.tick().press)
//            display.setPixel(32, 32);
//        display.sendBuffer();
//    }
    while(true) {
        auto selected = menu.menu_loop();
    }
    return 0;
}
