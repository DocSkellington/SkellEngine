#include "SkellEngine/states/State.h"

#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/systems/SystemManager.h"

namespace engine::states {
    State::State(StateManager &manager) :
        m_stateContext(manager.getContext()) {
    }

    State::~State() {

    }

    bool State::isTranscendant() const {
        return m_transcendant;
    }

    bool State::isTransparent() const {
        return m_transparent;
    }

    StateContext& State::getStateContext() {
        return m_stateContext;
    }

    const StateContext& State::getStateContext() const {
        return m_stateContext;
    }

    sf::View State::getView() const {
        return m_view;
    }

    void State::setView(const sf::View &view) {
        m_view = view;
    }

    State::Ptr State::createInstance(const std::string &name, StateManager &manager) {
        Ptr ptr = RegisteredStates::construct(name, manager);

        if (!ptr) {
            // TODO: ExternalState
        }

        return ptr;
    }
}