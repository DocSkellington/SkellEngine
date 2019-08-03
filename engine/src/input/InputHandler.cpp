#include "SkellEngine/input/InputHandler.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_fusion.h"

const auto AnyKey = sf::Keyboard::Key::KeyCount;
const auto AnyMouseButton = sf::Mouse::Button::ButtonCount;
const auto AnyJoystickButton = sf::Joystick::ButtonCount;
const auto AnyJoystickID = sf::Joystick::Count;

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

/**
 * \brief The default key/button is the "Any" key/button
 * \param event The event to modify
 */
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
    case sf::Event::EventType::JoystickMoved:
        event.joystickMove.joystickId = AnyJoystickID;
        break;
    default:
        break;
    }
}

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
        tmx::Logger::log("Input handler: error while parsing an input description: the 'type' field is absent from the input description. Description received: " + inputDescription.dump(4), tmx::Logger::Type::Warning);
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

    InputHandler::InputConnection InputHandler::connectInput(const std::string &eventType, nlohmann::json inputDescription) {
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
        return json;
    }

    void InputHandler::processAction(const thor::ActionContext<InputHandler::ActionId> &actionContext) {
        auto itr = m_actionIdToEventInformation.find(actionContext.actionId);
        if (itr == m_actionIdToEventInformation.end()) {
            tmx::Logger::log("Input handler: an input has triggered a registered callback but the callback was deleted before it could be called", tmx::Logger::Type::Warning);
            return;
        }

        const EventInformation &information = itr->second;
        // We check if the action must be effectively processed (if we are currently in an appropriate state)
        if (information.m_state != "all" && !m_context.stateManager->isCurrentState(information.m_state)) {
            return;
        }
        nlohmann::json json;

        auto jsonPayload = information.m_payload.begin();

        for (const auto& [input, hold] : information.m_input) {
            if (hold) { // Thor doesn't give the sf::Event when the event must be repeated (is marked as hold)
                jsonPayload++;
                continue;
            }

            const sf::Event &event = *actionContext.event;

            // We retrieve useful information from the SFML event
            if (input.type == event.type) {
                switch(input.type) {
                // sf::Event.size
                case sf::Event::EventType::Resized:
                    json["width"] = event.size.width;
                    json["height"] = event.size.height;
                    break;
                // sf::Event.mouseWheelScrolled
                case sf::Event::EventType::MouseWheelScrolled:
                    json["vertical"] = event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel;
                    json["delta"] = event.mouseWheelScroll.delta;
                    json["x"] = event.mouseWheelScroll.x;
                    json["y"] = event.mouseWheelScroll.y;
                    break;
                // sf::Event.mouseButton
                case sf::Event::EventType::MouseButtonPressed:
                case sf::Event::EventType::MouseButtonReleased:
                    json["x"] = event.mouseButton.x;
                    json["y"] = event.mouseButton.y;
                    json["button"] = event.mouseButton.button;
                    break;
                // sf::Event.mouseMove
                case sf::Event::EventType::MouseMoved:
                    json["x"] = event.mouseMove.x;
                    json["y"] = event.mouseMove.y;
                    break;
                // For joysticks, we always retrieve the ID
                // sf::Event.joystickMove
                case sf::Event::EventType::JoystickMoved:
                    json["position"] = event.joystickMove.position;
                    json["id"] = event.joystickMove.joystickId;
                    break;
                // sf::Event.joystickButton
                case sf::Event::EventType::JoystickButtonPressed:
                case sf::Event::EventType::JoystickButtonReleased:
                    json["id"] = event.joystickButton.joystickId;
                    json["button"] = event.joystickButton.button;
                    break;
                // sf::Event.joystickConnect
                case sf::Event::EventType::JoystickConnected:
                case sf::Event::EventType::JoystickDisconnected:
                    json["id"] = event.joystickConnect.joystickId;
                    break;
                // sf::Event.key
                case sf::Event::EventType::KeyPressed:
                case sf::Event::EventType::KeyReleased:
                    json["key"] = event.key.code;
                    break;
                // sf::Event without data (or without data to transfer)
                case sf::Event::EventType::Closed:
                case sf::Event::EventType::LostFocus:
                case sf::Event::EventType::GainedFocus:
                case sf::Event::EventType::MouseEntered:
                case sf::Event::EventType::MouseLeft:
                    // Nothing to do
                    break;
                default:
                    tmx::Logger::log("Input handler: error while handling an input: unknown input type. No information is extracted from the input.");
                    break;
                }
                break;
            }

            if (jsonPayload != information.m_payload.end()) {
                json = utilities::json_fusion(*jsonPayload, json);
            }
            jsonPayload++;
        }
        m_context.eventHandler->sendEvent(information.m_eventType, json);
    }

    InputHandler::EventInformation InputHandler::createEventInformation(const std::string &eventType, nlohmann::json &inputDescription, bool allowTables) const {
        sf::Event event;
        event.type = sf::Event::EventType::Count;
        nlohmann::json json;
        bool hold = false, lalt = false, lshift = false, lcontrol = false, ralt = false, rshift = false, rcontrol = false;
        std::string state = "all";

        if (inputDescription.is_string()) {
            event.type = stringToEventType(inputDescription.get<std::string>());
            setDefaultKeyButton(event);
        }
        else if (inputDescription.is_object()) {
            parseInputDescriptionObject(inputDescription, event, hold, lalt, lshift, lcontrol, ralt, rshift, rcontrol, state, json);
        }
        else if (inputDescription.is_array()) { // An array is an OR operation
            if (!allowTables) {
                tmx::Logger::log("Input handler: the input description is invalid: nested tables are not allowed", tmx::Logger::Type::Warning);
                return EventInformation();
            }

            EventInformation information;
            information.m_eventType = eventType;
            information.m_state = state;

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
            if (event.key.code == AnyKey) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.key.code, actionType);
            }
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
            break;
        case sf::Event::EventType::KeyReleased:
            holdActivated = false;
            if (event.key.code == AnyKey) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.key.code, actionType);
            }
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
            break;
        case sf::Event::EventType::MouseButtonPressed:
            holdActivated = hold;
            if (event.mouseButton.button == AnyMouseButton) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.mouseButton.button, actionType);
            }
            break;
        case sf::Event::EventType::MouseButtonReleased:
            holdActivated = false;
            if (event.mouseButton.button == AnyMouseButton) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.mouseButton.button, actionType);
            }
            break;
        case sf::Event::EventType::JoystickMoved:
            holdActivated = false;
            // Since Thor handles JoystickMoved as realtime events (and that means we can't retrieve the position of the joystick), we need to explicitly handle this kind of event
            action = thor::eventAction([event = event](const sf::Event &e) {
                // We need that :
                //  - the event is JoystickMoved
                //  - the ID of the controller is the one we want OR it's any controller
                //  - the axis is the one we want
                return e.type == sf::Event::EventType::JoystickMoved && (event.joystickMove.joystickId == AnyJoystickID || e.joystickMove.joystickId == event.joystickMove.joystickId) && e.joystickMove.axis == event.joystickMove.axis;
            });
            break;
        case sf::Event::EventType::JoystickButtonPressed:
            holdActivated = hold;
            if (event.joystickButton.button == sf::Joystick::ButtonCount) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(thor::JoystickButton(event.joystickButton.joystickId, event.joystickButton.button), actionType);
            }
            break;
        case sf::Event::EventType::JoystickButtonReleased:
            holdActivated = false;
            if (event.joystickButton.button == sf::Joystick::ButtonCount) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(thor::JoystickButton(event.joystickButton.joystickId, event.joystickButton.button), actionType);
            }
            break;
        default:
            return EventInformation();
            break;
        }

        return EventInformation(event, holdActivated, action, eventType, json, state);
    }

    InputHandler::EventInformation::EventInformation() {
    }

    InputHandler::EventInformation::EventInformation(const sf::Event &input, bool isHold, const thor::Action &action, const std::string &eventType, const nlohmann::json &payload, const std::string &state) :
        m_action(action),
        m_eventType(eventType),
        m_state(state) {
        m_input.emplace_back(input, isHold);
        m_payload.emplace_back(payload);
    }

    nlohmann::json InputHandler::EventInformation::toJSON() const {
        // TODO:
    }
}