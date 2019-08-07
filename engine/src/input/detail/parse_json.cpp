#include "SkellEngine/input/detail/parse_json.h"

#include <Thor/Input.hpp>
#include <Thor/Graphics/ToString.hpp>

#include "SkellEngine/tmxlite/Log.hpp"
#include "SkellEngine/input/detail/default.h"
#include "SkellEngine/input/detail/to_from_sfml_events.h"

namespace engine::input::detail {
    inline void parseInputDescriptionObjectKeyboard(nlohmann::json &inputDescription, sf::Event &event) {
        if (auto key = inputDescription.find("key"); key != inputDescription.end()) {
            if (key->is_string()) {
                try {
                    event.key.code = thor::toKeyboardKey(*key);
                }
                catch (const thor::StringConversionException &error) { // The key was invalid
                    if (*key == "Any" || *key == "any") {
                        setDefaultKeyButton(event);
                    }
                    else {
                        tmx::Logger::log("Input handler: error while parsing an input description: the 'key' field could not be converted to a keyboard key. Received key: " + key->get<std::string>(), tmx::Logger::Type::Warning);
                    }
                }
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'key' field could not be converted to a keyboard key. Received key: " + key->get<std::string>(), tmx::Logger::Type::Warning);
            }

            inputDescription.erase(key);
        }
    }

    inline void parseInputDescriptionObjectMouseButton(nlohmann::json &inputDescription, sf::Event &event) {
        if (auto mouseButton = inputDescription.find("mouse button") ; mouseButton != inputDescription.end()) {
            if (mouseButton->is_string()) {
                try {
                    event.mouseButton.button = thor::toMouseButton(*mouseButton);
                }
                catch (const thor::StringConversionException &error) { // The mouse button was invalid
                    if (*mouseButton == "Any" || *mouseButton == "any") {
                        setDefaultKeyButton(event);
                    }
                    else {
                        tmx::Logger::log("Input handler: error while parsing an input description: the 'mouse button' field could not be converted to a mouse button. Received: " + mouseButton->get<std::string>(), tmx::Logger::Type::Warning);
                    }
                }
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'mouse button' field could not be converted to a mouse button. Received: " + mouseButton->get<std::string>(), tmx::Logger::Type::Warning);
            }

            inputDescription.erase(mouseButton);
        }
    }

    inline void parseInputDescriptionObjectJoystickButton(nlohmann::json &inputDescription, sf::Event &event) {
        if (auto id = inputDescription.find("joystick id") ; id != inputDescription.end()) {
            if (id->is_number_integer()) {
                event.joystickButton.joystickId = *id;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'joystick id' field must be an unsigned integer", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(id);
        }
        else {
            event.joystickButton.joystickId = AnyJoystickID;
        }

        if (auto button = inputDescription.find("joystick button") ; button != inputDescription.end()) {
            if (button->is_number_unsigned()) {
                event.joystickButton.button = *button;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'joystick button' field must be an unsigned integer", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(button);
        }
    }

    inline void parseInputDescriptionObjectJoystickMove(nlohmann::json &inputDescription, sf::Event &event) {
        if (auto id = inputDescription.find("joystick id") ; id != inputDescription.end()) {
            if (id->is_number_integer()) {
                event.joystickMove.joystickId = *id;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'joystick id' field must be an unsigned integer", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(id);
        }
        else {
            event.joystickMove.joystickId = AnyJoystickID;
        }

        if (auto axis = inputDescription.find("axis") ; axis != inputDescription.end()) {
            if (axis->is_string()) {
                try {
                    event.joystickMove.axis = thor::toJoystickAxis(*axis);
                }
                catch (const thor::StringConversionException &error) { // The axis was invalid
                    event.type = sf::Event::EventType::Count;
                    tmx::Logger::log("Input handler: error while parsing an input description: the 'axis' field could not be converted to a joystick axis. Received: " + axis->get<std::string>(), tmx::Logger::Type::Warning);
                }
            }
            else {
                event.type = sf::Event::EventType::Count;
                tmx::Logger::log("Input handler: error while parsing an input description: the 'axis' field must be a string", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(axis);
        }
        else {
            event.type = sf::Event::EventType::Count;
            tmx::Logger::log("Input handler: while parsing the input description: the 'axis' field is not present for a JoystickMove input", tmx::Logger::Type::Warning);
        }
    }

    inline void parseInputDescriptionObjectModifiers(nlohmann::json &inputDescription, bool &lalt, bool &lshift, bool &lcontrol, bool &ralt, bool &rshift, bool &rcontrol) {
        if (auto modifier = inputDescription.find("shift") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lshift = rshift = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'shift' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("lshift") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lshift = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'lshift' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("rshift") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                rshift = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'rshift' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }

        if (auto modifier = inputDescription.find("alt") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lalt = ralt = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'alt' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("lalt") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lalt = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'lalt' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("ralt") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                ralt = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'ralt' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }

        if (auto modifier = inputDescription.find("control") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lcontrol = rcontrol = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'control' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("lcontrol") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                lcontrol = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'lcontrol' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
        if (auto modifier = inputDescription.find("rcontrol") ; modifier != inputDescription.end()) {
            if (modifier->is_boolean()) {
                rcontrol = *modifier;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'rcontrol' field must be a boolean", tmx::Logger::Type::Warning);
            }

            inputDescription.erase(modifier);
        }
    }

    void parseInputDescriptionObject(nlohmann::json &inputDescription, sf::Event &event, bool &hold, bool &lalt, bool &lshift, bool &lcontrol, bool &ralt, bool &rshift, bool &rcontrol, std::string &state, nlohmann::json &payload) {
        // First, we need to know the type of the sf::Event to create
        // Then, we parse the specific information needed for this type (and we remove the fields from the JSON object)
        // Finally, every unused field as considered as the JSON payload
        auto type = inputDescription.find("type");
        bool isJoystickMove = false;

        // First: the type of the input
        if (type != inputDescription.end() && type->is_string()) {
            try {
                event.type = stringToEventType(*type);
                setDefaultKeyButton(event);

                if (event.type == sf::Event::EventType::JoystickMoved) {
                    isJoystickMove = true;
                }
            }
            catch(const std::invalid_argument &error) { // The type of the event is not an SFML event
                std::string v = type->get<std::string>();
                std::transform(v.begin(), v.end(), v.begin(), [](const char& c) { return std::tolower(c); });

                if (v == "keyhold") {
                    event.type = sf::Event::EventType::KeyPressed;
                    hold = true;
                }
                else if (v == "mousebuttonhold") {
                    event.type = sf::Event::EventType::MouseButtonPressed;
                    hold = true;
                }
                else if (v == "joystickbuttonhold") {
                    event.type = sf::Event::EventType::JoystickButtonPressed;
                    hold = true;
                }
                else {
                    throw error;
                }
                setDefaultKeyButton(event);
            }
        }
        else {
            tmx::Logger::log("Input handler: error while parsing an input description: the type of the input must be present and must be a string", tmx::Logger::Type::Warning);
        }

        // Second: information on the input
        switch (event.type) {
        case sf::Event::EventType::KeyPressed:
        case sf::Event::EventType::KeyReleased:
            parseInputDescriptionObjectKeyboard(inputDescription, event);
            break;
        case sf::Event::EventType::MouseButtonPressed:
        case sf::Event::EventType::MouseButtonReleased:
            parseInputDescriptionObjectMouseButton(inputDescription, event);
            break;
        case sf::Event::EventType::JoystickButtonPressed:
        case sf::Event::EventType::JoystickButtonReleased:
            parseInputDescriptionObjectJoystickButton(inputDescription, event);
            break;
        case sf::Event::EventType::JoystickMoved:
            parseInputDescriptionObjectJoystickMove(inputDescription, event);
            break;
        default:
            break;
        }

        parseInputDescriptionObjectModifiers(inputDescription, lalt, lshift, lcontrol, ralt, rshift, rcontrol);

        if (event.type == sf::Event::EventType::Count) {
            tmx::Logger::log("Input handler: error while parsing an input description: the 'type' field is absent from the input description or an error occured (see above messages). Description received: " + inputDescription.dump(4), tmx::Logger::Type::Warning);
            return;
        }

        if (auto s = inputDescription.find("state") ; s != inputDescription.end()) {
            if (s->is_string()) {
                state = *s;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'state' valid must be a string. It default to 'all'.", tmx::Logger::Type::Warning);
                state = "all";
            }
        }
        else {
            state = "all";
        }

        // Third: JSON payload
        for (auto &[key, value] : inputDescription.items()) {
            payload[key] = value;
        }
    }
}