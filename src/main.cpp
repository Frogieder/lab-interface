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
    Menu menu = Menu(&display);

    multicore_launch_core1(core1_entry);
    /*****************************
     *            LOOP           *
     *****************************/
    while(true) {
//        display.clear();
//        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
//        pico_ssd1306::drawLine(&display,0, 0, 32, 32);
//        pico_ssd1306::drawText(&display, font_5x8, std::to_string((time_us_32()/100000)/10.0), 0, 0);
//        display.sendBuffer();
//        sleep_ms(200);

        auto selected = menu.menu_loop();
        pico_ssd1306::drawText(&display, font_5x8, std::to_string(selected), 32, 32);
        sleep_ms(2000);
        display.clear();

//        for (int y = 0; y < 64; y++) {
//            gpio_put(16, false);
//            display.clear();
//            pico_ssd1306::drawLine(&display,0, 0, 127, y);
// //            to_char_array<uint32_t>(time_us_32() / 1000000, word);
//            to_char_array((int32_t)(time_us_32() / 1000000), word);
//            pico_ssd1306::drawText(&display, font_8x8, word, 32, 32);
//            if (floor(log10(0)) != floor(log10(9))) {
//                pico_ssd1306::drawText(&display, font_8x8, "Meth no wrokz", 0, 40);
//            }
//            display.sendBuffer();
//            word[0] = 'E';
//            gpio_put(16, true);
//            sleep_ms(100);
//        }
    }
    return 0;
}
