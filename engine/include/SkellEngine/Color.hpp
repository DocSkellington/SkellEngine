#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>

namespace engine {
    /**
     * \brief A color in Red-Green-Blue-Alpha encoding
     */
    class Color {
    public:
        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color MAGENTA;
        static const Color CYAN;
        static const Color TRANSPARENT;

    public:
        Color() noexcept;
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) noexcept;
        explicit Color(std::uint32_t color) noexcept;

        operator sf::Color() const noexcept;

    public:
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
        std::uint8_t alpha;
    };
}