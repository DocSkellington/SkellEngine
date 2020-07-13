#include "SkellEngine/Color.hpp"

namespace engine {
    const Color Color::BLACK(0, 0, 0);
    const Color Color::WHITE(255, 255, 255);
    const Color Color::RED(255, 0, 0);
    const Color Color::GREEN(0, 255, 0);
    const Color Color::BLUE(0, 0, 255);
    const Color Color::YELLOW(255, 255, 0);
    const Color Color::MAGENTA(255, 0, 255);
    const Color Color::CYAN(0, 255, 255);
    const Color Color::TRANSPARENT(0, 0, 0, 0);

    Color::Color() noexcept :
        Color(0, 0, 0) {

    }

    Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept :
        red(r),
        green(g),
        blue(b),
        alpha(a) {

    }

    Color::Color(std::uint32_t color) noexcept :
        red(  (color & 0xFF000000) >> 24),
        green((color & 0x00FF0000) >> 16),
        blue( (color & 0x0000FF00) >> 8),
        alpha((color & 0x000000FF) >> 0) {

    }

    Color::operator sf::Color() const noexcept {
        return sf::Color(red, green, blue, alpha);
    }
}