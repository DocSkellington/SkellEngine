#include "SkellEngine/animations/detail/to_from_sfml_color.h"

#include <algorithm>

namespace engine::animations::detail {
    sf::Color stringToColor(std::string color) {
        std::transform(color.begin(), color.end(), color.begin(), ::tolower);
        if (color == "black") {
            return sf::Color::Black;
        }
        else if (color == "white") {
            return sf::Color::White;
        }
        else if (color == "red") {
            return sf::Color::Red;
        }
        else if (color == "green") {
            return sf::Color::Green;
        }
        else if (color == "blue") {
            return sf::Color::Blue;
        }
        else if (color == "yellow") {
            return sf::Color::Yellow;
        }
        else if (color == "mangenta") {
            return sf::Color::Magenta;
        }
        else if (color == "Cyan") {
            return sf::Color::Cyan;
        }
        else if (color == "transparent") {
            return sf::Color::Transparent;
        }
        // TODO: generic encoding
        return sf::Color();
    }
}