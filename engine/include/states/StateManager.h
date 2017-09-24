#pragma once

#include <list>
#include <utility>
#include <memory>
#include <typeindex>

#include "State.h"

/**
* \brief The states
*/
namespace engine::states {
    class StateManager final {
    public:
        explicit StateManager();
        ~StateManager();

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
            std::unique_ptr<State> state = make_unique<T>();
            state->onCreate();
            m_states.emplace_front(typeid(T), std::move(state));
        }

    private:
        std::list<std::pair<std::type_index, std::unique_ptr<State>>> m_states;
    };
}