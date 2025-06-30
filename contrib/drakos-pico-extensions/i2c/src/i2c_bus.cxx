#include "../include/dpe/i2c_bus.hxx"
using namespace dpe::i2c;

#include <pico/binary_info.h>
#include <pico/time.h>

#include <hardware/gpio.h>

I2cBus::I2cBus(i2c_inst_t *const i2c, uint const sda_pin, uint const scl_pin, uint const baud_rate)
    : i2c_{i2c} {
    i2c_init(i2c_, baud_rate);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    sleep_ms(50u);
}

I2cBus::~I2cBus() {
    i2c_deinit(i2c_);
}

// ReSharper disable once CppMemberFunctionMayBeConst
int I2cBus::write_blocking(uint8_t const address, uint8_t const *const data, size_t const length, bool const no_stop) {
    return i2c_write_blocking(i2c_, address, data, length, no_stop);
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool I2cBus::test(uint8_t const address) {
    if (is_reserved(address)) return false;

    uint8_t rx_data;
    return i2c_read_blocking(i2c_, address, &rx_data, 1u, false) >= 0u;
}

std::bitset<128u> I2cBus::scan(uint8_t const begin, uint8_t const end) {
    std::bitset<128u> result{};
    for (uint8_t n = begin; n < end; ++n) {
        if (test(n)) result.set(n);
    }
    return result;
}
