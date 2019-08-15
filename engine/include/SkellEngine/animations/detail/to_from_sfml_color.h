#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>

namespace engine::animations::detail {
    sf::Color stringToColor(std::string color);
    std::string colorToString(const sf::Color &color);
}