#include "i2c_lcd_display.hxx"

#include <dpe/led.hxx>
using namespace dpe::led;

#include <dpe/flags.hxx>

#include <cstdio>

#include <pico/binary_info.h>
#include <pico/stdio.h>
#include <pico/time.h>

#define I2C_SDA 20
#define I2C_SCL 21

#define LCD_I2C_ADDRESS 0x27

[[noreturn]] int main() {
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));

    Led led{GP{PICO_DEFAULT_LED_PIN}};
    led.off();

    stdio_init_all();
    sleep_ms(2'000u);

    dpe::i2c::I2cBus bus{i2c0, I2C_SDA, I2C_SCL, 100'000u};
    auto const found = bus.scan();
    for (uint8_t address = 0x00; address < 0x80; ++address) {
        if (found[address]) {
            std::printf("Found device at address 0x%02X!\n", address);
        }
    }
    if (!found[LCD_I2C_ADDRESS]) {
        std::printf("I2C LCD not found!\n");
        for (;;);
    }

    I2cLcdDisplay lcd{bus, LCD_I2C_ADDRESS};

    led.on();

    lcd.print("Hello, world!");
    lcd.set_cursor(0, 1);
    lcd.print("How are you?");

    sleep_ms(2'000u);

    for (;;) {
    }
}
