#include "SkellEngine/states/StateManager.h"

#include "SkellEngine/Context.h"

namespace engine::states {
    StateManager::StateManager(Context &context) :
        m_context(context) {
    }

    StateManager::~StateManager() {
        for (auto itr = m_states.begin() ; itr != m_states.end() ; ++itr) {
            if (itr->second)
                itr->second->onDestroy();
        }
    }

    void StateManager::update(sf::Int64 deltatime) {
        for (auto &state : m_states) {
            state.second->update(deltatime);

            if (!state.second->isTranscendant())
                break;
        }
    }

    void StateManager::draw(std::shared_ptr<sf::RenderWindow> &window) {
        for (auto &state : m_states) {
            state.second->draw(window);

            if (!state.second->isTransparent())
                break;
        }
    }

    void StateManager::handleEvent(sf::Event &event) {
        for (auto &state : m_states) {
            state.second->handleEvent(event);

            if(!state.second->isTranscendant())
                break;
        }
    }

    void StateManager::switchTo(const std::string &name) {
        // First, we check if the state is already in the list
        for (auto itr = m_states.begin() ; itr != m_states.end() ; itr++) {
            if (itr->first == name && itr->second) {
                // We have found it
                // We deactivate the front one
                m_states.front().second->deactivate();

                // We move the state to the front
                std::shared_ptr<State> state = itr->second;
                m_states.erase(itr);
                m_states.emplace_front(name, state);
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
        std::shared_ptr<State> state = State::createInstance(name, *this);
        state->onCreate();
        state->activate();
        m_states.emplace_front(name, std::move(state));
    }

    void StateManager::remove(const std::string &name) {
        m_toRemove.push_back(name);
    }

    void StateManager::processRemove() {
        while (m_toRemove.begin() != m_toRemove.end()) {
            // Looking for the state
            for (auto itr = m_states.begin() ; itr != m_states.end() ; ++itr) {
                if (itr->first == m_toRemove.front()) {
                    // Destroying the state
                    itr->second->onDestroy();
                    m_states.erase(itr);
                    break;
                }
            }

            // In every case, we remove the request
            m_toRemove.pop_front();
        }
    }

    Context &StateManager::getContext() {
        return m_context;
    }
}