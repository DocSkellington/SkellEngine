#include "states/State.h"

#include "states/StateManager.h"

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
}