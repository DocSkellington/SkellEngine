#include "SkellEngine/input/InputHandler.hpp"

#include "SkellEngine/Context.hpp"
#include "SkellEngine/utilities/json_fusion.hpp"
#include "SkellEngine/input/detail/to_from_sfml_events.hpp"
#include "SkellEngine/input/detail/parse_json.hpp"
#include "SkellEngine/input/detail/write_json.hpp"
#include "SkellEngine/input/detail/default.hpp"
#include "SkellEngine/states/StateManager.hpp"
#include "SkellEngine/events/EventHandler.hpp"
#include "SkellEngine/errors/InvalidJSON.hpp"

thor::Action applyModifiers(thor::Action action, bool lshift, bool lalt, bool lcontrol, bool rshift, bool ralt, bool rcontrol) {
    if (lshift) {
        action = thor::Action(sf::Keyboard::Key::LShift, thor::Action::ActionType::Hold) && action;
    }
    if (lalt) {
        action = thor::Action(sf::Keyboard::Key::LAlt, thor::Action::ActionType::Hold) && action;
    }
    if (lcontrol) {
        action = thor::Action(sf::Keyboard::Key::LControl, thor::Action::ActionType::Hold) && action;
    }
    if (rshift) {
        action = thor::Action(sf::Keyboard::Key::RShift, thor::Action::ActionType::Hold) && action;
    }
    if (ralt) {
        action = thor::Action(sf::Keyboard::Key::RAlt, thor::Action::ActionType::Hold) && action;
    }
    if (rcontrol) {
        action = thor::Action(sf::Keyboard::Key::RControl, thor::Action::ActionType::Hold) && action;
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
        EventInformation information;
        try {
            information = createEventInformation(eventType, inputDescription);
        }
        catch (const errors::InvalidJSON &e) {
            m_context.logger.logError("Input handler: an error occured during the creation of an input reaction. The input will not be connected.", e);
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

    void InputHandler::luaFunctions(sol::state &lua) {
        lua.new_usertype<InputConnection>("InputConnection",
            "isConnected", &InputConnection::isConnected,
            "disconnect", &InputConnection::disconnect
        );

        lua.new_usertype<InputHandler>("InputHandler",
            "connectInput", &InputHandler::connectInput,
            "loadConfiguration", &InputHandler::loadConfiguration,
            "saveConfiguration", &InputHandler::saveConfiguration
        );

        lua["game"]["inputHandler"] = this;
    }

    void InputHandler::processAction(const thor::ActionContext<InputHandler::ActionId> &actionContext) {
        auto itr = m_actionIdToEventInformation.find(actionContext.actionId);
        if (itr == m_actionIdToEventInformation.end()) {
            m_context.logger.log("Input handler: an input has triggered a registered callback but the callback was deleted before it could be called");
            return;
        }

        const EventInformation &information = itr->second;
        // We check if the action must be effectively processed (if we are currently in an appropriate state)
        if (information.m_state != "all" && !m_context.stateManager->isCurrentState(information.m_state)) {
            return;
        }
        nlohmann::json payload;

        for (const auto& input : information.m_input) {
            if (input.isHold) { // Thor doesn't give the sf::Event when the event must be repeated (is marked as hold)
                continue;
            }

            const sf::Event &event = *actionContext.event;

            // We retrieve useful information from the SFML event
            if (input.input.type == event.type) {
                switch(input.input.type) {
                // sf::Event.size
                case sf::Event::EventType::Resized:
                    payload["width"] = event.size.width;
                    payload["height"] = event.size.height;
                    break;
                // sf::Event.mouseWheelScrolled
                case sf::Event::EventType::MouseWheelScrolled:
                    payload["vertical"] = event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel;
                    payload["delta"] = event.mouseWheelScroll.delta;
                    payload["x"] = event.mouseWheelScroll.x;
                    payload["y"] = event.mouseWheelScroll.y;
                    break;
                // sf::Event.mouseButton
                case sf::Event::EventType::MouseButtonPressed:
                case sf::Event::EventType::MouseButtonReleased:
                    payload["x"] = event.mouseButton.x;
                    payload["y"] = event.mouseButton.y;
                    payload["button"] = event.mouseButton.button;
                    break;
                // sf::Event.mouseMove
                case sf::Event::EventType::MouseMoved:
                    payload["x"] = event.mouseMove.x;
                    payload["y"] = event.mouseMove.y;
                    break;
                // For joysticks, we always retrieve the ID
                // sf::Event.joystickMove
                case sf::Event::EventType::JoystickMoved:
                    payload["position"] = event.joystickMove.position;
                    payload["id"] = event.joystickMove.joystickId;
                    break;
                // sf::Event.joystickButton
                case sf::Event::EventType::JoystickButtonPressed:
                case sf::Event::EventType::JoystickButtonReleased:
                    payload["id"] = event.joystickButton.joystickId;
                    payload["button"] = event.joystickButton.button;
                    break;
                // sf::Event.joystickConnect
                case sf::Event::EventType::JoystickConnected:
                case sf::Event::EventType::JoystickDisconnected:
                    payload["id"] = event.joystickConnect.joystickId;
                    break;
                // sf::Event.key
                case sf::Event::EventType::KeyPressed:
                case sf::Event::EventType::KeyReleased:
                    payload["key"] = event.key.code;
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
                    m_context.logger.log("Input handler: error while handling an input: unknown input type. No information is extracted from the input.");
                    break;
                }
                break;
            }

            payload = utilities::json_fusion(payload, input.payload);
        }
        m_context.eventHandler->sendEvent(information.m_eventType, payload);
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
                throw errors::InvalidJSON("Input handler: the input description is invalid: nested tables are not allowed");
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
                    information.m_action = std::move(tempInformation.m_action);
                }
                else {
                    information.m_input.push_back(std::move(tempInformation.m_input.front()));
                    information.m_action = information.m_action || tempInformation.m_action;
                }

            }
            return information;
        }
        else {
            throw errors::InvalidJSON("Input handler: the input description is invalid: the description must be an object, an array or a string.");
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
            break;
        case sf::Event::EventType::KeyReleased:
            holdActivated = false;
            if (event.key.code == AnyKey) {
                action = thor::Action(event.type);
            }
            else {
                action = thor::Action(event.key.code, actionType);
            }
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

        action = applyModifiers(action, lshift, lalt, lcontrol, rshift, ralt, rcontrol);

        return EventInformation(event, holdActivated, action, eventType, json, state, ralt, rshift, rcontrol, lalt, lshift, lcontrol);
    }

    InputHandler::EventInformation::Input::Input(const sf::Event &input, const nlohmann::json &payload, bool isHold, bool ralt, bool rshift, bool rcontrol, bool lalt, bool lshift, bool lcontrol) :
        input(input),
        payload(payload),
        isHold(isHold),
        ralt(ralt),
        rshift(rshift),
        rcontrol(rcontrol),
        lalt(lalt),
        lshift(lshift),
        lcontrol(lcontrol) {
    }

    InputHandler::EventInformation::EventInformation() {
    }

    InputHandler::EventInformation::EventInformation(const sf::Event &input, bool isHold, const thor::Action &action, const std::string &eventType, const nlohmann::json &payload, const std::string &state, bool ralt, bool rshift, bool rcontrol, bool lalt, bool lshift, bool lcontrol) :
        m_action(action),
        m_eventType(eventType),
        m_state(state) {
        m_input.emplace_back(input, payload, isHold, ralt, rshift, rcontrol, lalt, lshift, lcontrol);
    }

    nlohmann::json InputHandler::EventInformation::toJSON() const {
        nlohmann::json json;

        if (m_input.size() == 1) {
            auto &input = m_input.front();
            json = writeInputDescriptionObject(input.input, input.isHold, input.payload, m_state, input.lalt, input.lshift, input.lcontrol, input.ralt, input.rshift, input.rcontrol);
        }
        else {
            for (const auto &input : m_input) {
                json.push_back(writeInputDescriptionObject(input.input, input.isHold, input.payload, m_state, input.lalt, input.lshift, input.lcontrol, input.ralt, input.rshift, input.rcontrol));
            }
        }

        return json;
    }
}