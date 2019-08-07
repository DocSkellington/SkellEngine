#include "SkellEngine/input/detail/to_from_sfml_events.h"

namespace engine::input::detail {
    sf::Event::EventType stringToEventType(std::string type) {
        std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) { return std::tolower(c); });

        static std::map<std::string, sf::Event::EventType> eventMap = {
            { "closed", sf::Event::EventType::Closed },
            { "resized", sf::Event::EventType::Resized },
            { "lostfocus", sf::Event::EventType::LostFocus },
            { "gainedfocus", sf::Event::EventType::GainedFocus },
            { "keypressed", sf::Event::EventType::KeyPressed },
            { "keyreleased", sf::Event::EventType::KeyReleased },
            { "mousewheelscrolled", sf::Event::EventType::MouseWheelScrolled },
            { "mousebuttonpressed", sf::Event::EventType::MouseButtonPressed },
            { "mousebuttonreleased", sf::Event::EventType::MouseButtonReleased },
            { "mousemoved", sf::Event::EventType::MouseMoved },
            { "mouseentered", sf::Event::EventType::MouseEntered },
            { "mouseleft", sf::Event::EventType::MouseLeft },
            { "joystickbuttonpressed", sf::Event::EventType::JoystickButtonPressed },
            { "joystickbuttonreleased", sf::Event::EventType::JoystickButtonReleased },
            { "joystickmoved", sf::Event::EventType::JoystickMoved },
            { "joystickconnected", sf::Event::EventType::JoystickConnected },
            { "joystickdisconnected", sf::Event::EventType::JoystickDisconnected }
        };

        auto itr = eventMap.find(type);

        if (itr == eventMap.end()) {
            throw std::invalid_argument("Input handler: invalid input type: " + type);
        }
        return itr->second;
    }

    std::string eventTypeToString(sf::Event::EventType type, bool isHold) {
        static std::map<sf::Event::EventType, std::string> eventMap = {
            { sf::Event::EventType::Closed, "closed"},
            { sf::Event::EventType::Resized, "resized"},
            { sf::Event::EventType::LostFocus, "lostfocus"},
            { sf::Event::EventType::GainedFocus, "gainedfocus"},
            { sf::Event::EventType::KeyPressed, "keypressed"},
            { sf::Event::EventType::KeyReleased, "keyreleased"},
            { sf::Event::EventType::MouseWheelScrolled, "mousewheelscrolled"},
            { sf::Event::EventType::MouseButtonPressed, "mousebuttonpressed"},
            { sf::Event::EventType::MouseButtonReleased, "mousebuttonreleased"},
            { sf::Event::EventType::MouseMoved, "mousemoved"},
            { sf::Event::EventType::MouseEntered, "mouseentered"},
            { sf::Event::EventType::MouseLeft, "mouseleft"},
            { sf::Event::EventType::JoystickButtonPressed, "joystickbuttonpressed"},
            { sf::Event::EventType::JoystickButtonReleased, "joystickbuttonreleased"},
            { sf::Event::EventType::JoystickMoved, "joystickmoved"},
            { sf::Event::EventType::JoystickConnected, "joystickconnected"},
            { sf::Event::EventType::JoystickDisconnected,"joystickdisconnected" }
        };

        auto itr = eventMap.find(type);

        if (itr == eventMap.end()) {
            throw std::invalid_argument("Input handler: invalid input type: " + std::to_string(type));
        }

        if (isHold) {
            switch (type) {
            case sf::Event::EventType::KeyPressed:
                return "keyhold";
            case sf::Event::EventType::MouseButtonPressed:
                return "mousebuttonhold";
            case sf::Event::EventType::JoystickButtonPressed:
                return "joystickbuttonhold";
            default:
                break;
            }
        }
        return itr->second;
    }
}