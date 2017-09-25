#include "states/StateManager.h"

#include "states/MainMenuState.h"

namespace engine::states {
    StateManager::StateManager() {
        switchTo<MainMenuState>();
    }

    StateManager::~StateManager() {
        for (auto itr = m_states.begin() ; itr != m_states.end() ; ++itr) {
            if (itr->second)
                itr->second->onDestroy();
        }
    }

    void StateManager::update(float deltatime) {
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
}