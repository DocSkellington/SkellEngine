#include "SkellEngine/input/InputHandler.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_fusion.h"
#include "SkellEngine/input/detail/to_from_sfml_events.h"
#include "SkellEngine/input/detail/parse_json.h"
#include "SkellEngine/input/detail/write_json.h"
#include "SkellEngine/input/detail/default.h"

thor::Action applyModifiers(thor::Action action, thor::Action::ActionType actionType, bool lshift, bool lalt, bool lcontrol, bool rshift, bool ralt, bool rcontrol) {
    if (lshift) {
        action = action && thor::Action(sf::Keyboard::Key::LShift, actionType);
    }
    if (lalt) {
        action = action && thor::Action(sf::Keyboard::Key::LAlt, actionType);
    }
    if (lcontrol) {
        action = thor::Action(sf::Keyboard::Key::LControl, thor::Action::ActionType::Hold) && action;
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
    using namespace detail;

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
            json[eventInformation.m_eventType] = eventInformation.toJSON();
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
        case sf::Event::EventType::MouseWheelScrolled:
            holdActivated = false;
            action = thor::Action(event.type);
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol); // TODO: crash
            break;
        case sf::Event::EventType::Closed:
        case sf::Event::EventType::Resized:
        case sf::Event::EventType::LostFocus:
        case sf::Event::EventType::GainedFocus:
        case sf::Event::EventType::TextEntered:
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
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
            break;
        case sf::Event::EventType::MouseButtonReleased:
            holdActivated = false;
            if (event.mouseButton.button == AnyMouseButton) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.mouseButton.button, actionType);
            }
            action = applyModifiers(action, actionType, lshift, lalt, lcontrol, rshift, ralt, rcontrol);
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

    InputHandler::EventInformation::Input::Input() {
    }

    InputHandler::EventInformation::Input::Input(const sf::Event &input, bool isHold) :
        input(input),
        isHold(isHold) {
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
        nlohmann::json json;

        if (m_input.size() == 1) {
            json = writeInputDescriptionObject(m_input.front().input, m_input.front().isHold, m_payload, m_state);
        }
        else {
            for (auto [inputItr, payloadItr] = std::tuple{m_input.begin(), m_payload.begin()} ; inputItr != m_input.end() && payloadItr != m_payload.end() ; ++inputItr, ++payloadItr) {
                json.push_back(writeInputDescriptionObject(inputItr->input, inputItr->isHold, *payloadItr, m_state));
            }
        }

        return json;
    }
}