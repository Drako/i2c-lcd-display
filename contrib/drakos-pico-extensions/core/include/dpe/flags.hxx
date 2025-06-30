#pragma once

#include <concepts>
#include <type_traits>

namespace dpe::core::flags {
    namespace detail {
        template<typename E>
        struct is_scoped_enum final {
            // ReSharper disable once CppFunctionIsNotImplemented
            template<typename U = E>
            static auto test(int, U * = nullptr) -> std::false_type;

            // ReSharper disable once CppFunctionIsNotImplemented
            static auto test(...) -> std::true_type;

            constexpr static bool value = std::is_enum_v<E> && decltype(test(std::declval<E>()))::value;
        };
    }

    template<typename E>
    concept scoped_enum = detail::is_scoped_enum<E>::value;

    template<scoped_enum E>
    struct flag_traits;

    template<typename E>
    concept flag_enum = requires
    {
        requires std::same_as<std::remove_const_t<decltype(flag_traits<E>::mask)>, std::underlying_type_t<E> >;
    };

    template<flag_enum E>
    constexpr E operator |(E const lhs, E const rhs) {
        using UT = std::underlying_type_t<E>;
        return static_cast<E>(static_cast<UT>(lhs) | static_cast<UT>(rhs));
    }

    template<flag_enum E>
    constexpr E &operator |=(E &lhs, E const rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    template<flag_enum E>
    constexpr E operator &(E const lhs, E const rhs) {
        using UT = std::underlying_type_t<E>;
        return static_cast<E>(static_cast<UT>(lhs) & static_cast<UT>(rhs));
    }

    template<flag_enum E>
    constexpr E &operator &=(E &lhs, E const rhs) {
        lhs = lhs & rhs;
        return lhs;
    }

    template<flag_enum E>
    constexpr E operator ~(E const rhs) {
        using UT = std::underlying_type_t<E>;
        return static_cast<E>(~static_cast<UT>(rhs) & flag_traits<E>::mask);
    }

    constexpr static struct {
        template<flag_enum E>
        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator E() const { // NOLINT(*-explicit-constructor)
            using UT = std::underlying_type_t<E>;
            return static_cast<E>(static_cast<UT>(0));
        }
    } no_flags;

    template<flag_enum E>
    struct flags final {
        E value;

        constexpr bool contain_all(E const other_flags) const {
            return (value & other_flags) == other_flags;
        }

        constexpr bool contain_any(E const other_flags) const {
            return (value & other_flags) != no_flags;
        }

        constexpr flags set(E const other_flags) const {
            return {value | other_flags};
        }

        constexpr flags unset(E const other_flags) const {
            return {value & ~other_flags};
        }

        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator E() const { // NOLINT(*-explicit-constructor)
            return value;
        }

        explicit constexpr operator bool() const {
            return value != no_flags;
        }

        bool operator!() const {
            return value == no_flags;
        }
    };

    template<flag_enum E>
    flags(E) -> flags<E>;
}
