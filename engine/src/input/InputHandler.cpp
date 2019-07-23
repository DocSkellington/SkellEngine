#include "SkellEngine/input/InputHandler.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_fusion.h"

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

void parseInputDescriptionObject(const nlohmann::json &inputDescription, sf::Event &event, bool &hold, bool &lalt, bool &lshift, bool &lcontrol, bool &ralt, bool &rshift, bool &rcontrol, float &threshold, bool &above, nlohmann::json &json) {
    auto type = inputDescription.find("type");
    bool isJoystickMove = false;

    if (type != inputDescription.end() && type->is_string()) {
        try {
            event.type = stringToEventType(*type);
            if (event.type == sf::Event::EventType::JoystickMoved) {
                isJoystickMove = true;
            }
        }
        catch(const std::invalid_argument &error) {
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
        }
    }
    else {
        tmx::Logger::log("Input handler: error while parsing an input description: the type of the input must be present and must be a string", tmx::Logger::Type::Warning);
    }

    for (auto &[key, value] : inputDescription.items()) {
        if (key == "type") {
        }
        else if (key == "key") {
            if (value.is_string()) {
                event.key.code = thor::toKeyboardKey(value);
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'key' field could not be converted to a keyboard key. Received key: " + value.get<std::string>(), tmx::Logger::Type::Warning);
            }
        }
        else if (key == "mouse button") {
            if (value.is_string()) {
                event.mouseButton.button = thor::toMouseButton(value);
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the 'mouse button' field could not be converted to a keyboard key. Received key: " + value.get<std::string>(), tmx::Logger::Type::Warning);
            }
        }
        else if (key == "shift" || key == "lshift") {
            if (value.is_boolean()) {
                lshift = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "alt" || key == "lalt") {
            if (value.is_boolean()) {
                lalt = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "control" || key == "lcontrol") {
            if (value.is_boolean()) {
                lcontrol = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "rshift") {
            if (value.is_boolean()) {
                rshift = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "ralt") {
            if (value.is_boolean()) {
                ralt = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "rcontrol") {
            if (value.is_boolean()) {
                rcontrol = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "joystick id") {
            if (value.is_number_integer()) {
                if (isJoystickMove) {
                    event.joystickMove.joystickId = value;
                }
                else {
                    event.joystickButton.joystickId = value;
                }
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be an unsigned integer", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "axis") {
            if (value.is_string()) {
                if (isJoystickMove) {
                    event.joystickMove.axis = thor::toJoystickAxis(value);
                }
                else {
                    tmx::Logger::log("Input handler: error while parsing an input description: the 'axis' field can only be used for JoystickMoved event", tmx::Logger::Type::Warning);
                }
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a string", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "threshold") {
            if (value.is_number_float()) {
                threshold = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a float", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "above") {
            if (value.is_boolean()) {
                above = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be a boolean", tmx::Logger::Type::Warning);
            }
        }
        else if (key == "joystick button") {
            if (value.is_number_unsigned()) {
                event.joystickButton.button = value;
            }
            else {
                tmx::Logger::log("Input handler: error while parsing an input description: the '" + key + "' field must be an unsigned integer", tmx::Logger::Type::Warning);
            }
        }
        else {
            json[key] = value;
        }
    }
    if (event.type == sf::Event::EventType::Count) {
        tmx::Logger::log("Input handler: error while parsing an input description: the 'type' field is absent from the input description. Description received: " + inputDescription.get<std::string>(), tmx::Logger::Type::Warning);
    }
}

thor::Action applyModifiers(thor::Action action, thor::Action::ActionType actionType, bool lshift, bool lalt, bool lcontrol, bool rshift, bool ralt, bool rcontrol) {
    if (lshift) {
        action = action && thor::Action(sf::Keyboard::Key::LShift, actionType);
    }
    if (lalt) {
        action = action && thor::Action(sf::Keyboard::Key::LAlt, actionType);
    }
    if (lcontrol) {
        action = action && thor::Action(sf::Keyboard::Key::LControl, actionType);
    }
    if (rshift) {
        action = action && thor::Action(sf::Keyboard::Key::RShift, actionType);
    }
    if (ralt) {
        action = action && thor::Action(sf::Keyboard::Key::RAlt, actionType);
    }
    if (rcontrol) {
        action = action && thor::Action(sf::Keyboard::Key::RControl, actionType);
    }
    return action;
}

namespace engine::input {
    InputHandler::InputConnection::InputConnection(InputHandler &inputHandler, const thor::Connection &connection, ActionId id) :
        m_inputHandler(inputHandler),
        m_connection(connection),
        m_id(id) {

    }

    void InputHandler::InputConnection::disconnect() {
        if (isConnected()) {
            m_connection.disconnect();
            m_inputHandler.m_freedIds.push(m_id);
            m_inputHandler.m_actionMap.removeAction(m_id);
            m_inputHandler.m_actionIdToEventInformation.erase(m_id);
        }
    }

    bool InputHandler::InputConnection::isConnected() const {
        return m_connection.isConnected();
    }

    InputHandler::InputHandler(Context &context) :
        m_context(context) {

    }

    InputHandler::InputConnection InputHandler::connectInput(const std::string &eventType, const nlohmann::json &inputDescription) {
        EventInformation information = createEventInformation(eventType, inputDescription);
        bool error = information.m_input.size() == 0;
        if (!error) {
            for (const auto& [input, hold] : information.m_input) {
                if (input.type == sf::Event::EventType::Count) {
                    error = true;
                    break;
                }
            }
        }
        if (error) {
            tmx::Logger::log("Input handler: an error occured during the creation of an input reaction. The input will not be connected.", tmx::Logger::Type::Warning);
            return InputConnection(*this, thor::Connection(), 0);
        }

        ActionId id;
        if (m_freedIds.size() != 0) {
            id = m_freedIds.front();
            m_freedIds.pop();
        }
        else {
            id = m_nextId++;
        }

        m_actionMap[id] = information.m_action;
        thor::Connection connection = m_callbackSystem.connect(id, std::bind(&InputHandler::processAction, this, std::placeholders::_1));
        m_actionIdToEventInformation[id] = std::move(information);
        
        return InputConnection(*this, connection, id);
    }

    void InputHandler::proccess(const sf::Event &event) {
        m_actionMap.pushEvent(event);
    }

    void InputHandler::clearInputs() {
        m_actionMap.clearEvents();
    }

    void InputHandler::triggerEvents() {
        m_actionMap.invokeCallbacks(m_callbackSystem, nullptr);
    }

    void InputHandler::loadConfiguration(const nlohmann::json &configuration) {
        for (auto& [key, value] : configuration.items()) {
            connectInput(key, value);
        }
    }

    nlohmann::json InputHandler::saveConfiguration() const {
        nlohmann::json json;
        for (auto &[id, eventInformation] : m_actionIdToEventInformation) {
            utilities::json_fusion(json, eventInformation.toJSON());
        }
    }

    void InputHandler::processAction(const thor::ActionContext<InputHandler::ActionId> &actionContext) {
        auto itr = m_actionIdToEventInformation.find(actionContext.actionId);
        if (itr == m_actionIdToEventInformation.end()) {
            tmx::Logger::log("Input handler: an input has triggered a registered callback but the callback was deleted before it could be called", tmx::Logger::Type::Warning);
            return;
        }

        const EventInformation &information = itr->second;
        nlohmann::json json;

        auto jsonPayload = information.m_payload.begin();

        for (const auto& [input, hold] : information.m_input) {
            if (hold) { // Thor doesn't give the sf::Event when the event must be repeated (is marked as hold)
                jsonPayload++;
                continue;
            }

            if (input.type == actionContext.event->type) {
                switch(input.type) {
                // sf::Event.size
                case sf::Event::EventType::Resized:
                    json["width"] = input.size.width;
                    json["height"] = input.size.height;
                    break;
                // sf::Event.mouseWheelScrolled
                case sf::Event::EventType::MouseWheelScrolled:
                    json["vertical"] = input.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel;
                    json["delta"] = input.mouseWheelScroll.delta;
                    json["x"] = input.mouseWheelScroll.x;
                    json["y"] = input.mouseWheelScroll.y;
                    break;
                // sf::Event.mouseButton
                case sf::Event::EventType::MouseButtonPressed:
                case sf::Event::EventType::MouseButtonReleased:
                    json["x"] = input.mouseButton.x;
                    json["y"] = input.mouseButton.y;
                    break;
                // sf::Event.mouseMove
                case sf::Event::EventType::MouseMoved:
                    json["x"] = input.mouseMove.x;
                    json["y"] = input.mouseMove.y;
                    break;
                // sf::Event.joystickMove
                case sf::Event::EventType::JoystickMoved:
                    json["position"] = input.joystickMove.position;
                    break;
                // sf::Event.joystickConnect
                case sf::Event::EventType::JoystickConnected:
                case sf::Event::EventType::JoystickDisconnected:
                    json["id"] = input.joystickConnect.joystickId;
                    break;
                // sf::Event without data (or without data to transfer)
                case sf::Event::EventType::Closed:
                case sf::Event::EventType::LostFocus:
                case sf::Event::EventType::GainedFocus:
                case sf::Event::EventType::KeyPressed:
                case sf::Event::EventType::KeyReleased:
                case sf::Event::EventType::MouseEntered:
                case sf::Event::EventType::MouseLeft:
                case sf::Event::EventType::JoystickButtonPressed:
                case sf::Event::EventType::JoystickButtonReleased:
                    // Nothing to do
                    break;
                default:
                    tmx::Logger::log("Input handler: error while handling an input: unknown input type. No information is extracted from the input.");
                    break;
                }
                break;
            }
            jsonPayload++;
        }

        if (jsonPayload != information.m_payload.end()) {
            json = utilities::json_fusion(*jsonPayload, json);
        }
        m_context.eventHandler->sendEvent(information.m_eventType, json);
    }

    InputHandler::EventInformation InputHandler::createEventInformation(const std::string &eventType, const nlohmann::json &inputDescription, bool allowTables) const {
        sf::Event event;
        event.type = sf::Event::EventType::Count;
        nlohmann::json json;
        bool hold = false, lalt = false, lshift = false, lcontrol = false, ralt = false, rshift = false, rcontrol = false, above = false;
        float threshold = 0;

        if (inputDescription.is_string()) {
            event.type = stringToEventType(inputDescription.get<std::string>());
        }
        else if (inputDescription.is_object()) {
            parseInputDescriptionObject(inputDescription, event, hold, lalt, lshift, lcontrol, ralt, rshift, rcontrol, threshold, above, json);
        }
        else if (inputDescription.is_array()) {
            if (!allowTables) {
                tmx::Logger::log("Input handler: the input description is invalid: nested tables are not allowed", tmx::Logger::Type::Warning);
                return EventInformation();
            }
            EventInformation information;
            information.m_eventType = eventType;
            for (auto description = inputDescription.begin() ; description != inputDescription.end() ; ++description) {
                EventInformation tempInformation = createEventInformation(eventType, *description, false);

                if (tempInformation.m_input.size() == 0) { // error
                    return EventInformation();
                }

                // We concatenate the information from the current description in the table to the already processed descriptions
                if (description == inputDescription.begin()) {
                    information.m_input = std::move(tempInformation.m_input);
                    information.m_payload = std::move(tempInformation.m_payload);
                    information.m_action = std::move(tempInformation.m_action);
                }
                else {
                    information.m_input.push_back(std::move(tempInformation.m_input.front()));
                    information.m_payload.push_back(std::move(tempInformation.m_payload.front()));
                    information.m_action = information.m_action || tempInformation.m_action;
                }

            }
            return information;
        }
        else {
            tmx::Logger::log("Input handler: the input description is invalid: the description must be an object, an array or a string.", tmx::Logger::Type::Warning);
            return EventInformation();
        }

        thor::Action::ActionType actionType;
        switch(event.type) {
        case sf::Event::EventType::JoystickButtonReleased:
        case sf::Event::EventType::KeyReleased:
        case sf::Event::EventType::MouseButtonReleased:
            actionType = thor::Action::ActionType::ReleaseOnce;
            break;
        case sf::Event::EventType::JoystickButtonPressed:
        case sf::Event::EventType::KeyPressed:
        case sf::Event::EventType::MouseButtonPressed:
            actionType = hold ? thor::Action::ActionType::Hold : thor::Action::ActionType::PressOnce;
            break;
        default:
            break;
        }

        thor::Action action;
        bool holdActivated = false;

        // We construct the appropriate action for each event type
        switch(event.type) {
        case sf::Event::EventType::Closed:
        case sf::Event::EventType::Resized:
        case sf::Event::EventType::LostFocus:
        case sf::Event::EventType::GainedFocus:
        case sf::Event::EventType::TextEntered:
        case sf::Event::EventType::MouseWheelScrolled:
        case sf::Event::EventType::MouseMoved:
        case sf::Event::EventType::MouseEntered:
        case sf::Event::EventType::MouseLeft:
        case sf::Event::EventType::JoystickConnected:
        case sf::Event::EventType::JoystickDisconnected:
            holdActivated = false;
            action = thor::Action(event.type);
            break;
        case sf::Event::EventType::KeyPressed:
            holdActivated = hold;
            action = thor::Action(event.key.code, actionType);
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
            break;
        case sf::Event::EventType::KeyReleased:
            holdActivated = false;
            action = thor::Action(event.key.code, actionType);
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
            break;
        case sf::Event::EventType::MouseButtonPressed:
            holdActivated = hold;
            action = thor::Action(event.mouseButton.button, actionType);
            break;
        case sf::Event::EventType::MouseButtonReleased:
            holdActivated = false;
            action = thor::Action(event.mouseButton.button, actionType);
            break;
        case sf::Event::EventType::JoystickMoved:
            holdActivated = false;
            action = thor::Action(thor::JoystickAxis(event.joystickMove.joystickId, event.joystickMove.axis, threshold, above));
            break;
        case sf::Event::EventType::JoystickButtonPressed:
            holdActivated = hold;
            action = thor::Action(thor::JoystickButton(event.joystickButton.joystickId, event.joystickButton.button), actionType);
            break;
        case sf::Event::EventType::JoystickButtonReleased:
            holdActivated = false;
            action = thor::Action(thor::JoystickButton(event.joystickButton.joystickId, event.joystickButton.button), actionType);
            break;
        default:
            return EventInformation();
            break;
        }

        return EventInformation(event, holdActivated, action, eventType, json);
    }

    InputHandler::EventInformation::EventInformation() {
    }

    InputHandler::EventInformation::EventInformation(const sf::Event &input, bool isHold, const thor::Action &action, const std::string &eventType, const nlohmann::json &payload) :
        m_action(action),
        m_eventType(eventType) {
        m_input.emplace_back(input, isHold);
        m_payload.emplace_back(payload);
    }

    nlohmann::json InputHandler::EventInformation::toJSON() const {
        // TODO:
    }
}