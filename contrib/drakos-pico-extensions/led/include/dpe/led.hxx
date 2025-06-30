#pragma once

#include <pico/config.h>
#include <pico/types.h>

#if __has_include(<pico/cyw43_arch.h>)
#include <pico/cyw43_arch.h>
#endif

namespace dpe::led {
    /**
     * State of a regular LED.
     */
    enum class State : uint8_t {
        Off, //< Turned off.
        On, //< Turned on.
    };

    /**
     * Simple struct representing a GPIO pin.
     */
    struct GP final {
        uint pin;
    };

#ifdef _PICO_CYW43_ARCH_H
    /**
     * Simple struct representing the pin on the CYW43 chip connected to the builtin LED.
     */
    struct CYW43 final {
        /**
         * Declare that the LED instance should initialize and deinitialize the CYW43 chip.
         * Set this to false if the chip is not just used for the LED, but also for Wi-Fi or bluetooth.
         */
        bool exclusive = true;
    };
#endif // _PICO_CYW43_ARCH_H

    namespace detail {
        template<typename PinType>
        class LedImplementation;

        template<>
        class LedImplementation<GP> {
        public:
            // ReSharper disable once CppMemberFunctionMayBeConst
            void set_state(State const state) {
                gpio_put(pin_, state == State::On);
            }

            [[nodiscard]] State get_state() const {
                return gpio_get(pin_) ? State::On : State::Off;
            }

            // ReSharper disable once CppMemberFunctionMayBeConst
            explicit LedImplementation(GP const pin): pin_{pin.pin} {
                gpio_init(pin_);
                gpio_set_dir(pin_, GPIO_OUT);
            }

            // ReSharper disable once CppMemberFunctionMayBeConst
            ~LedImplementation() {
                gpio_deinit(pin_);
            }

        private:
            uint pin_ = 0u;
        };

#ifdef _PICO_CYW43_ARCH_H
        template<>
        class LedImplementation<CYW43> {
        public:
            // ReSharper disable once CppMemberFunctionMayBeConst
            // ReSharper disable once CppMemberFunctionMayBeStatic
            void set_state(State const state) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state == State::On);
            }

            // ReSharper disable once CppMemberFunctionMayBeStatic
            [[nodiscard]] State get_state() const {
                return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN) ? State::On : State::Off;
            }

            // ReSharper disable once CppMemberFunctionMayBeConst
            explicit LedImplementation(CYW43 const cyw43): exclusive_{cyw43.exclusive} {
                // ReSharper disable once CppDFAConstantConditions
                if (exclusive_) cyw43_arch_init();
            }

            // ReSharper disable once CppMemberFunctionMayBeConst
            ~LedImplementation() {
                if (exclusive_) cyw43_arch_deinit();
            }

        private:
            bool exclusive_;
        };
#endif // _PICO_CYW43_ARCH_H
    }

    /**
     * Class representing an LED.
     */
    template<typename PinType = GP>
    class Led final : public detail::LedImplementation<PinType> {
    public:
        /**
         * Construct an instance connected to the given GPIO pin.
         * @param pin The GPIO pin connected to an actual LED.
         */
        using detail::LedImplementation<PinType>::LedImplementation;

        /**
         * Turn the LED on or off.
         * @param state The new state of the LED.
         */
        using detail::LedImplementation<PinType>::set_state;

        /**
         * Get the current state of the LED.
         * @return The current state of the LED.
         */
        using detail::LedImplementation<PinType>::get_state;

        /**
         * Turn the LED on.
         */
        void on() {
            set_state(State::On);
        }

        /**
         * Turn the LED off.
         */
        void off() {
            set_state(State::Off);
        }

        /**
         * Check if the LED is currently turned on.
         * @return true, if the LED is turned on, false otherwise.
         */
        [[nodiscard]] bool is_on() const {
            return get_state() == State::On;
        }

        /**
         * Check if the LED is currently turned off.
         * @return true, if the LED is turned off, false otherwise.
         */
        [[nodiscard]] bool is_off() const {
            return get_state() == State::Off;
        }

    private:
        uint pin_;
    };

    template<typename PinType>
    Led(PinType) -> Led<PinType>;
}
