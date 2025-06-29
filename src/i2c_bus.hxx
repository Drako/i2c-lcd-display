#pragma once

#include <ranges>

#include <pico/types.h>

#include <hardware/i2c.h>

class I2cBus final {
public:
    constexpr static uint DEFAULT_BAUD_RATE = 100'000u; // 100 kHz

    I2cBus(i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint baud_rate = DEFAULT_BAUD_RATE);

    ~I2cBus();

    int write_blocking(uint8_t address, uint8_t const *data, size_t length, bool no_stop = false);

    int write_blocking(uint8_t const address, uint8_t const data, bool const no_stop = false) {
        return write_blocking(address, &data, 1, no_stop);
    }

    template<std::ranges::contiguous_range Range>
    int write_blocking(uint8_t const address, Range const &data, bool const no_stop = false) {
        return write_blocking(address, std::ranges::data(data), std::ranges::size(data), no_stop);
    }

private:
    i2c_inst_t *i2c_;
};
