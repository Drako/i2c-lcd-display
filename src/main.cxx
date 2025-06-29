#include "i2c_lcd_display.hxx"

#include <cstdio>

#include <pico/binary_info.h>
#include <pico/cyw43_arch.h>
#include <pico/stdio.h>
#include <pico/time.h>

#define I2C_SDA 16
#define I2C_SCL 17

#define LCD_I2C_ADDRESS 0x27

[[noreturn]] int main() {
    cyw43_arch_init();
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);

    stdio_init_all();
    sleep_ms(2'000u);

    I2cBus bus{i2c0, I2C_SDA, I2C_SCL};
    I2cLcdDisplay lcd{bus, LCD_I2C_ADDRESS};

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    lcd.print("Hello, world!");
    lcd.set_cursor(0, 1);
    lcd.print("How are you?");

    for (;;) {
    }
}
