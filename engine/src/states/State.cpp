#include "SkellEngine/states/State.h"

#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/states/ExternState.h"

namespace engine::states {
    State::State(StateManager &manager, const std::string &stateName) :
        m_transcendant(false),
        m_transparent(false),
        m_stateContext(manager.getContext(), stateName),
        m_storeEventConnections(*manager.getContext().eventHandler) {
    }
    
    State::State(StateManager &manager, const std::string &stateName, bool isTranscendant, bool isTransparent) :
        m_transcendant(isTranscendant),
        m_transparent(isTransparent),
        m_stateContext(manager.getContext(), stateName),
        m_storeEventConnections(*manager.getContext().eventHandler) {
    }

    State::~State() {
        getStoreEventConnections().clearEventConnections();
    }

    void State::handleEvent(const sf::Event &event) {
        m_stateContext.gui->handleEvent(event);
    }

    bool State::isTranscendant() const {
        return m_transcendant;
    }

    bool State::isTransparent() const {
        return m_transparent;
    }

    void State::setIsTranscendant(bool isTranscendant) {
        m_transcendant = isTranscendant;
    }

    void State::setIsTransparent(bool isTransparent) {
        m_transparent = isTransparent;
    }

    StateContext& State::getStateContext() {
        return m_stateContext;
    }

    const StateContext& State::getStateContext() const {
        return m_stateContext;
    }

    State::Ptr State::createInstance(const std::string &name, StateManager &manager) {
        Ptr ptr = RegisteredStates::construct(name, manager, name);

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