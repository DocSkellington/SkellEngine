#pragma once

#include <list>
#include <utility>
#include <memory>
#include <typeindex>

#include <SFML/Graphics.hpp>

#include "SkellEngine/states/State.h"

namespace engine {
    class Context;
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
         * \brief Gives the sf::Event to the states starting from the front and until it reaches a non-transcendant state
         * \param event The event
         */
        void handleEvent(const sf::Event &event);

        /**
        * \brief Draws the states starting from the front and until it reaches a non-transparent state.
        * \param target The target in which the manager must draw
        */
        void draw(sf::RenderTarget &target);

        /**
         * \brief Asks to change the front state to the one of the given name
         * 
         * The request is processed when processSwitchToAndRemove is called (at the end of the game loop). If a new request comes in before the old one is processed, only the new request is effectively processed
         * 
         * If the state does not exist yet, it will be created
         * 
         * The state must have been registered
         * \param name The name of the state to switch to
         */
        void switchTo(const std::string &name);

        /**
        * \brief Asks to remove the given state.
        * 
        * The requests are processed when processSwitchToAndRemove is called (at the end of the game loop)
        * \param name The name of the state to remove
        */
        void remove(const std::string &name);

        /**
        * \brief Process the switchTo and remove requests received
        */
        void processSwitchToAndRemove();

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

        /**
         * \brief Registers Lua functions
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    private:
        using StateInList = std::pair<std::string, State::Ptr>;

    private:
        std::list<StateInList> m_states;
        std::list<std::string> m_toRemove;
        std::string m_switchTo;
        Context &m_context;
    };
}