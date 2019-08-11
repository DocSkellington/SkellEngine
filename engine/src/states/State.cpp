#include "SkellEngine/states/State.h"

#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/states/ExternState.h"

namespace engine::states {
    State::State(StateManager &manager) :
        m_stateContext(manager.getContext()),
        m_storeEventConnections(*manager.getContext().eventHandler) {
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
            try {
                ptr = std::make_shared<ExternState>(manager, name);
            }
            catch (const std::exception &e) {
                tmx::Logger::logError("ExternState: impossible to load the Lua script for state " + name + ". The state won't be added to the state manager", e);
                return nullptr;
            }
        }

        return ptr;
    }

    events::StoreEventConnections& State::getStoreEventConnections() {
        return m_storeEventConnections;
    }

    const events::StoreEventConnections& State::getStoreEventConnections() const {
        return m_storeEventConnections;
    }

    events::EventConnection State::registerCallback(const std::string &eventType, const events::EventHandler::callbackSignature &callback, const std::string &state) {
        return getStoreEventConnections().registerCallback(eventType, callback, state);
    }
}