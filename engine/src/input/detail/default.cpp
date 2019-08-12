#include "SkellEngine/input/detail/default.h"

namespace engine::input::detail {
    void setDefaultKeyButton(sf::Event &event) {
        switch (event.type) {
        case sf::Event::EventType::KeyPressed:
        case sf::Event::EventType::KeyReleased:
            event.key.code = AnyKey;
            break;
        case sf::Event::EventType::MouseButtonPressed:
        case sf::Event::EventType::MouseButtonReleased:
            event.mouseButton.button = AnyMouseButton;
            break;
        case sf::Event::EventType::JoystickButtonPressed:
        case sf::Event::EventType::JoystickButtonReleased:
            event.joystickButton.button = AnyJoystickButton;
            event.joystickButton.joystickId = AnyJoystickID;
            break;
        case sf::Event::EventType::JoystickMoved:
            event.joystickMove.joystickId = AnyJoystickID;
            break;
        default:
            break;
        }
    }
}