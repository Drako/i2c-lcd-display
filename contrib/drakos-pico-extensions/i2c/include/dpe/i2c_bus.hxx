#pragma once

#include <bitset>
#include <ranges>

#include <pico/types.h>

#include <hardware/i2c.h>

namespace dpe::i2c {
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

        /**
         * Check if a device is responding to the given address.
         * @param address The address to check.
         * @return true, if a device responded, false otherwise.
         */
        bool test(uint8_t address);

        /**
         * Scan the given address range for devices.
         * @param begin The start of the range to scan (inclusive).
         * @param end The end of the range to scan (exclusive).
         * @return A bitset where the bits representing found devices are set to true.
         */
        std::bitset<128u> scan(uint8_t begin = 0x00, uint8_t end = 0x80);

    private:
        i2c_inst_t *i2c_;

        constexpr static bool is_reserved(uint8_t const address) {
            constexpr uint8_t reserved_mask = 0x78;
            uint8_t const masked_address = address & reserved_mask;
            return masked_address == 0x00 || masked_address == reserved_mask;
        }
    };
}
