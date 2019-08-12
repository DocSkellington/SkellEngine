#pragma once

#include <map>
#include <list>
#include <queue>

#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>
#include <Thor/Input.hpp>

#include "SkellEngine/events/Event.h"

namespace engine {
    class Context;
}

/**
 * \brief How to handle to keyboard/mouse/joystick inputs and other user-triggered events
 */
namespace engine::input {
    /**
     * \brief Handles the input with the user such as window resized, key pressed, etc.
     * 
     *  - <b>Input</b> is used for player-generated inputs. That is, when the player presses a button, moves the mouse, etc. Typically, it's a sf::Event object.
     *  - <b>Event</b> is used for the events (engine::events::Event) sent by the input handler through the event handler when a received input matches a registered input.
     * 
     * Typically, the connections are loaded from a JSON file describing the input
     * \see engine::files::GameDescription::MediaDescription, @ref json_game_description and TODO: the Markdown documentation for the file description
     * \see engine::events::EventHandler for how to subsribe for an event
     * \todo TODO: add graphical sprites for PS4, Xbox, Gamecube and Switch Pro controllers
     * \todo TODO: save/load configuration
     */
    class InputHandler {
    private:
        using ActionId = unsigned long;

    public:
        /**
         * \brief Allows to remove a callback from the input handler
         * 
         * If the connection object dies (the variable reaches the end of its scope), the actual connection is <b>not</b> removed (the callback will still be called when an appropriate input is triggered).
         */
        class InputConnection {
            friend class InputHandler;

        public:
            /**
             * \brief Disconnects the input from the input handler
             */
            void disconnect();

            /**
             * \brief Checks whether the instance currently references a callback
             * \return True iff the instance references a registered callback in the input handler
             */
            bool isConnected() const;

        private:
            InputConnection(InputHandler &inputHandler, const thor::Connection &connection, ActionId id);
        
        private:
            InputHandler &m_inputHandler;
            thor::Connection m_connection;
            ActionId m_id;
        };

    public:
        /**
         * \brief Constructs the input handler
         * \param context The context
         */
        InputHandler(Context &context);
        InputHandler(const InputHandler&) = delete;

        /**
         * \brief Adds a new input for the given event type
         * \param eventType The event type
         * \param inputDescription The description of the input
         * \return An object that can be used to disconnect the input. If you do not need that, you can just ignore it.
         */
        InputConnection connectInput(const std::string &eventType, nlohmann::json inputDescription);

        /**
         * \brief Adds an input (an sf::Event) for the current frame
         * \param event The event
         */
        void proccess(const sf::Event &event);

        /**
         * \brief Clears the events that have been temporaly stored
         */
        void clearInputs();

        /**
         * \brief Triggers the events
         */
        void triggerEvents();

        /**
         * \brief Loads configuration from file
         * \param configuration The configuration described by a JSON object
         */
        void loadConfiguration(const nlohmann::json &configuration);

        /**
         * \brief Gives the current configuration of the input handler in a JSON object
         * \return The JSON describing the current configuration
         */
        nlohmann::json saveConfiguration() const;

        /**
         * \brief Registers Lua functions
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    private:

        /**
         * \brief Stores some information about the input->event mapping, such as the JSON payload, the event type and the different possible inputs
         */
        struct EventInformation {
            /**
             * \brief Stores an input
             * 
             * Stores the SFML event, the JSON payload and the state of the modifiers
             */
            struct Input {
                /**
                 * \brief The constructor
                 * \param input The SFML event
                 * \param payload The JSON payload
                 * \param isHold Whether the event is sent each frame as long as the input is active
                 * \param ralt Whether the right alt key must be pressed
                 * \param rshift Whether the right shift key must be pressed
                 * \param rcontrol Whether the right control key must be pressed
                 * \param lalt Whether the left alt key must be pressed
                 * \param lshift Whether the left shift key must be pressed
                 * \param lcontrol Whether the left control key must be pressed
                 */
                Input(const sf::Event &input, const nlohmann::json &payload, bool isHold, bool ralt, bool rshift, bool rcontrol, bool lalt, bool lshift, bool lcontrol);
                /** @{ */
                /**
                 * \brief The different parts of the input
                 */
                sf::Event input;
                nlohmann::json payload;
                bool isHold, ralt, rshift, rcontrol, lalt, lshift, lcontrol;
                /** @} */
            };
            EventInformation();
            EventInformation(const sf::Event &input, bool isHold, const thor::Action &action, const std::string& eventType, const nlohmann::json &payload, const std::string &state, bool ralt, bool rshift, bool rcontrol, bool lalt, bool lshift, bool lcontrol);

            nlohmann::json toJSON() const;

            std::list<Input> m_input;
            thor::Action m_action;
            std::string m_eventType;
            std::string m_state;
        };

    private:
        /**
         * \brief Processes an action triggered by Thor
         * \param actionContext The context of the action
         */
        void processAction(const thor::ActionContext<ActionId>& actionContext);

        /**
         * \brief Processes the JSON description and create an EventInformation
         * \param eventType The type of the event to send
         * \param inputDescription The description of the input
         * \param allowTables True iff tables are allowed in the description. Tables are only allowed at the first level
         */
        EventInformation createEventInformation(const std::string &eventType, nlohmann::json &inputDescription, bool allowTables = true) const;
    
    private:
        Context& m_context;
        thor::ActionMap<ActionId> m_actionMap;
        thor::ActionMap<ActionId>::CallbackSystem m_callbackSystem;
        std::map<ActionId, EventInformation> m_actionIdToEventInformation;
        ActionId m_nextId;
        std::queue<ActionId> m_freedIds;
    };
}