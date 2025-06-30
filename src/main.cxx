#include "i2c_lcd_display.hxx"

#include <dpe/led.hxx>
using namespace dpe::led;

#include <dpe/flags.hxx>

#include <cstdio>

#include <pico/binary_info.h>
#include <pico/stdio.h>
#include <pico/time.h>

#define I2C_SDA 16
#define I2C_SCL 17

#define LCD_I2C_ADDRESS 0x27

[[noreturn]] int main() {
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));

    Led led{CYW43{}};
    led.off();

    stdio_init_all();
    sleep_ms(2'000u);

    dpe::i2c::I2cBus bus{i2c0, I2C_SDA, I2C_SCL};
    I2cLcdDisplay lcd{bus, LCD_I2C_ADDRESS};

    led.on();

    lcd.print("Hello, world!");
    lcd.set_cursor(0, 1);
    lcd.print("How are you?");

    for (;;) {
    }
}
