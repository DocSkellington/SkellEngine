#pragma once

#include <list>
#include <utility>
#include <memory>
#include <typeindex>

#include <SFML/Graphics.hpp>

#include "State.h"

namespace engine {
    struct Context;
}

/**
* \brief The states
* \see REGISTER_STATE to easily register a new state in the engine 
*/
namespace engine::states {
    /**
    * \brief Handles the different states of the game.
    *
    * Updates and draws the states that must be updated/rendered (depending on their transparent/transcendant parameter).
    * It also provides functions to add or remove a state.
    * States are stored like in a stack, allowing to process them from the front until the end (if needed)
    *
    */
    class StateManager final {
    public:
        /**
         * \brief The constructor
         * \param context A reference to the context of the engine
         */
        explicit StateManager(Context &context);
        StateManager(const StateManager&) = delete;
        ~StateManager();

        /**
        * \brief Updates the states starting from the front and until it reaches a non-transcendant state.
        * \param deltatime The time since the last update
        */
        void update(sf::Int64 deltatime);

        /**
        * \brief Draws the states starting from the front and until it reaches a non-transparent state.
        * \param window The RenderWindow in which the manager must draw
        */
        void draw(std::shared_ptr<sf::RenderWindow> &window);

        /**
         * \brief Handles the event
         * \todo TODO: to keep?
         * \param event The event to process
         */
        void handleEvent(sf::Event &event);

        /**
         * \brief Changes the front state to the one of the given name
         * 
         * If the state does not exist yet, it is created
         * 
         * The state must have been registered
         * \param name The name of the state to switch to
         */
        void switchTo(const std::string &name);

        /**
        * \brief Asks to remove the given state.
        * \param name The name of the state to remove
        */
        void remove(const std::string &name);

        /**
        * \brief Process the remove requests received from remove().
        *
        * A state is removed iff it is in the stack of states.
        */
        void processRemove();

        /**
         * \brief Gives the context
         * \return The context
         */
        Context &getContext();

        /**
         * \brief Gives the name of the state that is currently in front
         * \return The name of the front state
         */
        const std::string &getCurrentState() const;

        /**
         * \brief True iff the given state name is the same as the name of the current front state
         * \return True iff state is the name of the current front state
         */
        bool isCurrentState(const std::string &state) const;

    private:
        using StateInList = std::pair<std::string, State::Ptr>;

    private:
        std::list<StateInList> m_states;
        std::list<std::string> m_toRemove;
        Context &m_context;
    };
}