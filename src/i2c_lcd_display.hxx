#pragma once

#include "i2c_bus.hxx"

#include <pico/types.h>

enum class LcdCommand : uint8_t {
    CLEAR_DISPLAY = 0x01,
    RETURN_HOME = 0x02,
    ENTRY_MODE_SET = 0x04,
    DISPLAY_CONTROL = 0x08,
    CURSOR_SHIFT = 0x10,
    FUNCTION_SET = 0x20,
    SET_CG_RAM_ADDR = 0x40,
    SET_DD_RAM_ADDR = 0x80,
};

class I2cLcdDisplay final {
public:
    I2cLcdDisplay(I2cBus &i2c, uint8_t address);

    void clear();

    void home();

    void print(char const *str);

    void set_cursor(uint8_t col, uint8_t row);

private:
    I2cBus &i2c_;
    uint8_t const address_;

    // mid level
    void command(LcdCommand const cmd) {
        send(static_cast<uint8_t>(cmd), 0x00);
    }

    void write(uint8_t const value) {
        constexpr uint8_t REGISTER_SELECT = 0x01;
        send(value, REGISTER_SELECT);
    }

    // low level
    void expander_write(uint8_t data);

    void pulse_enable(uint8_t data);

    void write_4_bits(uint8_t data);

    void send(uint8_t value, uint8_t mode);
};
