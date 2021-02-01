#pragma once

#include <type_traits>

namespace engine::gui {
    enum class TextStyle : unsigned int {
        Regular         = 0,
        Bold            = 1 << 0,
        Italic          = 1 << 1,
        Underlined      = 1 << 2,
        StrikeThrough   = 1 << 3
    };

    inline TextStyle operator| (const TextStyle &lhs, const TextStyle &rhs) {
        using T = std::underlying_type_t<TextStyle>;
        return static_cast<TextStyle>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline TextStyle& operator|= (TextStyle &lhs, const TextStyle &rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    inline TextStyle operator& (const TextStyle &lhs, const TextStyle &rhs) {
        using T = std::underlying_type_t<TextStyle>;
        return static_cast<TextStyle>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }
}