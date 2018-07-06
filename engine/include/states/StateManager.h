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
        explicit StateManager(engine::Context &context);
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
         * \param event The event to process
         */
        void handleEvent(sf::Event &event);

        /**
        * \brief Changes the front state to the one provided.
        *
        * If the state does not exist, it is created.
        * \tparam T The type of the state
        */
        template <typename T>
        void switchTo() {
            // First, we check if the state is already in the list
            for (auto itr = m_states.begin() ; itr != m_states.end() ; itr++) {
                if (itr->first == typeid(T) && itr->second) {
                    // We have found it
                    // We deactivate the front one
                    m_states.front().second->deactivate();

                    // We move the state to the front
                    std::unique_ptr<State> state = std::move(itr->second);
                    m_states.erase(itr);
                    m_states.emplace_front(typeid(*state), std::move(state));
                    // And we activate it
                    m_states.front().second->activate();

                    return;
                }
            }

            // We haven't found it. We will create a new state
            // We deactivate the front one (if it exists)
            if (m_states.size() != 0)
                m_states.front().second->deactivate();

            // Creation
            std::unique_ptr<State> state = std::make_unique<T>(*this);
            state->onCreate();
            state->activate();
            m_states.emplace_front(typeid(T), std::move(state));
        }

        /**
        * \brief Asks to remove the given state.
        * \tparam The type of the state to remove.
        */
        template <typename T>
        void remove() {
            m_toRemove.push_back(typeid(T));
        }

        /**
        * \brief Process the remove requests received from remove().
        *
        * A state is removed iff it is in the stack of states.
        */
        void processRemove();

        Context &getContext();

    private:
        std::list<std::pair<std::type_index, std::unique_ptr<State>>> m_states;
        std::list<std::type_index> m_toRemove;
        Context &m_context;
    };
}