#include "SkellEngine/animations/detail/to_from_sfml_color.hpp"

#include <algorithm>

#include <iostream>
#include <sstream>

namespace engine::animations::detail {
    sf::Color jsonToColor(const nlohmann::json &color) {
        if (color.is_string()) {
            return stringToColor(color);
        }
        else if (color.is_number_unsigned()) {
            if (0 <= color && color <= 255) {
                return sf::Color(color, color, color);
            }
            else {
                throw std::invalid_argument("JSON to sf::Color conversion: invalid color: as an unsigned integer, the color must be in [0, 255]");
            }
        }
        else if (color.is_object()) {
            sf::Color col;
            if (auto r = color.find("r") ; r != color.end() && r->is_number() && 0 <= *r && *r <= 255) {
                col.r = *r;
            }
            if (auto g = color.find("g") ; g != color.end() && g->is_number() && 0 <= *g && *g <= 255) {
                col.g = *g;
            }
            if (auto b = color.find("b") ; b != color.end() && b->is_number() && 0 <= *b && *b <= 255) {
                col.b = *b;
            }
            if (auto a = color.find("a") ; a != color.end() && a->is_number() && 0 <= *a && *a <= 255) {
                col.a = *a;
            }
            return col;
        }
        else if (color.is_array()) {
            sf::Color col;
            for (unsigned int i = 0 ; i < color.size() ; i++) {
                if (color[i].is_number_unsigned() && 0 <= color[i] && color[i] <= 255) {
                    switch (i) {
                    case 0:
                        col.r = color[i];
                        break;
                    case 1:
                        col.g = color[i];
                        break;
                    case 2:
                        col.b = color[i];
                        break;
                    case 3:
                        col.b = color[i];
                        break;
                    default:
                        throw std::invalid_argument("JSON to sf::Color conversion: invalid color: a color described by an array must have at most 4 values (red, green, blue and alpha)");
                    }
                }
                else {
                    throw std::invalid_argument("JSON to sf::Color conversion: invalid color: the values in an array must be integers in [0,255]");
                }
            }
            return col;
        }
        else {
            throw std::invalid_argument("JSON to sf::Color conversion: invalid color: the color must be described by a string, a number, an object or an array");
        }
    }

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
        else { // Hexadecimal notation
            if (6 <= color.length() && color.length() <= 9) {
                std::size_t i = 0;
                // We skip the #
                if (color.at(0) == '#') {
                    i = 1;
                }

                int r = std::stoul(color.substr(i, 2), nullptr, 16);
                int g = std::stoul(color.substr(i + 2, 2), nullptr, 16);
                int b = std::stoul(color.substr(i + 4, 2), nullptr, 16);
                int a = 255;

                if (8 <= color.length()) {
                    // We have the alpha value as well
                    a = std::stoul(color.substr(i + 6, 2), nullptr, 16);
                }

                return sf::Color(r, g, b, a);
            }
            else {
                throw std::invalid_argument("JSON to sf::Color conversion: invalid color: the length of the string must be between 6 (just the three channels) and 9 (the # and the four channels)");
            }
        }
    }

    std::string colorToString(const sf::Color &color) {
        std::stringstream stream;
        stream << "#" << std::hex << (unsigned int)color.r << (unsigned int)color.g << (unsigned int)color.b << (unsigned int)color.a;
        std::string hexa(stream.str());
        return hexa;
    }
}