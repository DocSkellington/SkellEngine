#pragma once

#include <map>
#include <list>
#include <queue>

#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>
#include <Thor/Input.hpp>

#include "SkellEngine/events/Event.h"

namespace engine {
    struct Context;
}

/**
 * \brief How to handle to keyboard/mouse/joystick inputs and other user-triggered events
 */
namespace engine::input {
    /**
     * \brief Handles the input with the user such as window resized, key pressed, etc.
     * 
     *  - <b>Input</b> is used for player-generated inputs. That is, when the player presses a button, moves the mouse, etc.
     *  - <b>Event</b> is used for the events (engine::events::Event) sent by the input handler through the event handler when a received input matches a registered input
     * \todo TODO: add graphical sprites for PS4, Xbox, Gamecube and Switch Pro controllers
     * \todo TODO: save/load configuration
     */
    class InputHandler {
    private:
        using ActionId = unsigned long;

    public:
        /**
         * \brief Allows to remove a callback from the input handler
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
         */
        InputConnection connectInput(const std::string &eventType, const nlohmann::json& inputDescription);

        /**
         * \brief Adds an input (an sf::Event) for the current frame
         * \param The event
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

        nlohmann::json saveConfiguration() const;

    private:

        struct EventInformation {
            EventInformation();
            EventInformation(const sf::Event &input, bool isHold, const thor::Action &action, const std::string& eventType, const nlohmann::json &payload);

            nlohmann::json toJSON() const;

            std::list<std::pair<sf::Event, bool>> m_input;
            thor::Action m_action;
            std::string m_eventType;
            std::list<nlohmann::json> m_payload;
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
        EventInformation createEventInformation(const std::string &eventType, const nlohmann::json &inputDescription, bool allowTables = true) const;
    
    private:
        Context& m_context;
        thor::ActionMap<ActionId> m_actionMap;
        thor::ActionMap<ActionId>::CallbackSystem m_callbackSystem;
        std::map<ActionId, EventInformation> m_actionIdToEventInformation;
        ActionId m_nextId;
        std::queue<ActionId> m_freedIds;
    };
}