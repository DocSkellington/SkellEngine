#include "SkellEngine/states/State.h"

#include "SkellEngine/states/StateManager.h"

namespace engine::states {
    State::State(StateManager &manager) :
        m_stateManager(manager) {
    }

    State::~State() {

    }

    bool State::isTranscendant() const {
        return m_transcendant;
    }

    bool State::isTransparent() const {
        return m_transparent;
    }

    StateManager& State::getStateManager() const {
        return m_stateManager;
    }

    sf::View State::getView() const {
        return m_view;
    }

    void State::setView(const sf::View &view) {
        m_view = view;
    }
}