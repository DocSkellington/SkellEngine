#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <nlohmann/json.hpp>

namespace engine::animations::detail {
    sf::Color jsonToColor(const nlohmann::json &color);
    sf::Color stringToColor(std::string color);
    std::string colorToString(const sf::Color &color);
}