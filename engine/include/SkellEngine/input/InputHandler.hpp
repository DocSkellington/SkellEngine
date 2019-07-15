#pragma once

#include <map>
#include <list>

#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>

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
     * \todo TODO: add a function to convert JSON to an sf::Event
     * \todo TODO: save/load configuration
     */
    class InputHandler {
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
         * \param input The input
         * \todo TODO: change sf::Event for nlohmann::json
         */
        void addInput(const std::string &eventType, const sf::Event &input);

        /**
         * \brief Processes an sf::Event and triggers SkellEngine events if needed
         * \param The event
         */
        void proccess(const sf::Event &event) const;

        /**
         * \brief Test if the input is active
         * 
         * For example, if the input is tied to a key, the function checks if the key is currently pressed
         */
        bool isActive(const std::string &eventType);

    private:
        /**
         * \brief Maps one event to associated input
         */
        class InputMapping {
        public:
            /**
             * \brief Constructs an input mapping. When the input is triggered, an event without information is sent
             */
            InputMapping();
            /**
             * \brief Constructs an input mapping that sends an event with some information.
             * \param json The information to send with the event
             */
            InputMapping(const nlohmann::json &json);

            /**
             * \brief Adds a new input
             * \param event The sf::Event
             */
            void addInput(const sf::Event& event);

            /**
             * \brief Processes the given SFML event
             * \param event The SFML event
             * \return A pair with a boolean (true iff at least one of the associated sf::Event is triggered) and a JSON object containing information on the sf::Event (if applicable)
             */
            std::pair<bool, nlohmann::json> process(const sf::Event &event) const;

            /**
             * \brief For mouse, keyboard and joystick events, directly test if the bouton or key defined by at least one of the events is pressed 
             * \return True iff at least one of the events is currently active
             */
            bool isActive() const;

            /**
             * \brief Gives the JSON object describing the information to add in the event to send
             * \return The JSON object
             */
            const nlohmann::json &getJSON() const;
        
        private:
            std::list<sf::Event> m_inputs;
            const nlohmann::json m_json;
        };
    
    private:
        Context& m_context;
        std::map<std::string, InputMapping> m_inputMappings;
    };
}