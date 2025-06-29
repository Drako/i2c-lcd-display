#include "i2c_lcd_display.hxx"

#include <pico/binary_info.h>
#include <pico/time.h>

#include <hardware/gpio.h>

constexpr LcdCommand operator|(LcdCommand const cmd, uint8_t const data) {
    return static_cast<LcdCommand>(static_cast<uint8_t>(cmd) | data);
}

I2cLcdDisplay::I2cLcdDisplay(
    I2cBus &i2c,
    uint8_t const address
): i2c_{i2c}, address_{address} {
    expander_write(0x08);
    sleep_ms(1'000u);

    write_4_bits(0x03 << 4);
    sleep_us(4500u);
    write_4_bits(0x03 << 4);
    sleep_us(4500u);
    write_4_bits(0x03 << 4);
    sleep_us(150u);
    write_4_bits(0x02 << 4);

    constexpr uint8_t DISPLAY_FUNCTION = 0x08; // 0x00 = 5x8DOTS | 0x00 = 4BIT_MODE | 0x08 = 2LINE
    command(LcdCommand::FUNCTION_SET | DISPLAY_FUNCTION);
    constexpr uint8_t DISPLAY_FLAGS = 0x04; // 0x04 = DISPLAY ON | 0x00 = CURSOR OFF | 0x00 = BLINK OFF
    command(LcdCommand::DISPLAY_CONTROL | DISPLAY_FLAGS);

    clear();

    constexpr uint8_t DISPLAY_MODE = 0x02; // 0x02 = ENTRY LEFT | 0x00 = ENTRY SHIFT DECREMENT
    command(LcdCommand::ENTRY_MODE_SET | DISPLAY_MODE);

    home();
}

void I2cLcdDisplay::clear() {
    command(LcdCommand::CLEAR_DISPLAY);
    sleep_ms(2u);
}

void I2cLcdDisplay::home() {
    command(LcdCommand::RETURN_HOME);
    sleep_ms(2u);
}

void I2cLcdDisplay::print(char const *str) {
    for (char const *c = str; *c != '\0'; ++c) {
        write(*c);
    }
}

void I2cLcdDisplay::set_cursor(uint8_t const col, uint8_t const row) {
    constexpr uint8_t row_offsets[] = {0x00, 0x40};
    if (row > 1) return;
    command(LcdCommand::SET_DD_RAM_ADDR | col + row_offsets[row]);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void I2cLcdDisplay::expander_write(uint8_t const data) {
    constexpr uint8_t WITH_BACKLIGHT = 0x08;
    uint8_t const raw = data | WITH_BACKLIGHT;
    i2c_.write_blocking(address_, raw);
}

void I2cLcdDisplay::pulse_enable(uint8_t const data) {
    constexpr uint8_t ENABLE_FLAG = 0x04;
    expander_write(data | ENABLE_FLAG);
    sleep_us(1u);

    expander_write(data & ~ENABLE_FLAG);
    sleep_us(50u);
}

void I2cLcdDisplay::write_4_bits(uint8_t const data) {
    expander_write(data);
    pulse_enable(data);
}

void I2cLcdDisplay::send(uint8_t const value, uint8_t const mode) {
    uint8_t const high_nibble = value & 0xF0;
    uint8_t const low_nibble = value << 4 & 0xF0;
    write_4_bits(high_nibble | mode);
    write_4_bits(low_nibble | mode);
}
