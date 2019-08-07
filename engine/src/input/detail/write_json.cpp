#include "SkellEngine/input/detail/write_json.h"

#include <Thor/Input.hpp>

#include "SkellEngine/input/detail/to_from_sfml_events.h"
#include "SkellEngine/utilities/json_fusion.h"

namespace engine::input::detail {
    nlohmann::json writeInputDescriptionObject(const sf::Event &event, bool isHold, nlohmann::json payload, const std::string &state, bool lalt, bool lshift, bool lcontrol, bool ralt, bool rshift, bool rcontrol) {
        nlohmann::json json;

        json["type"] = eventTypeToString(event.type, isHold);
        json["state"] = state;

        json["lalt"] = lalt;
        json["lshift"] = lshift;
        json["lcontrol"] = lcontrol;
        json["ralt"] = ralt;
        json["rshift"] = rshift;
        json["rcontrol"] = rcontrol;

        switch(event.type) {
        case sf::Event::EventType::KeyPressed:
        case sf::Event::EventType::KeyReleased:
            writeInputDescriptionObjectKeyboard(json, event);
            break;
        case sf::Event::EventType::MouseButtonPressed:
        case sf::Event::EventType::MouseButtonReleased:
            writeInputDescriptionObjectMouseButton(json, event);
            break;
        case sf::Event::EventType::JoystickButtonPressed:
        case sf::Event::EventType::JoystickButtonReleased:
            writeInputDescriptionObjectJoystickButton(json, event);
            break;
        case sf::Event::EventType::JoystickMoved:
            writeInputDescriptionObjectJoystickMove(json, event);
            break;
        default:
            break;
        }

        return utilities::json_fusion(json, payload);
    }

    void writeInputDescriptionObjectKeyboard(nlohmann::json &json, const sf::Event &event) {
        json["key"] = thor::toString(event.key.code);
    }

    void writeInputDescriptionObjectMouseButton(nlohmann::json &json, const sf::Event &event) {
        json["mouse button"] = thor::toString(event.mouseButton.button);
    }

    void writeInputDescriptionObjectJoystickButton(nlohmann::json &json, const sf::Event &event) {
        json["joystick id"] = event.joystickButton.joystickId;
        json["joystick button"] = event.joystickButton.button;
    }

    void writeInputDescriptionObjectJoystickMove(nlohmann::json &json, const sf::Event &event) {
        json["joystick id"] = event.joystickMove.joystickId;
        json["axis"] = thor::toString(event.joystickMove.axis);
    }
}