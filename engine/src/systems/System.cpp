#include "SkellEngine/systems/System.h"

#include <cctype>

#include <SkellEngine/tmxlite/Log.hpp>
#include "SkellEngine/systems/ExternSystem.h"
#include "SkellEngine/errors/ConstructorNotValid.h"
#include "SkellEngine/systems/SystemManager.h"

namespace engine::systems {
    System::System(SystemManager& manager) :
        m_manager(manager),
        m_storeEventConnections(*manager.getContext().context.eventHandler) {
    }

    System::~System() {
        getStoreEventConnections().clearEventConnections();
    }

    bool System::addEntity(entities::Entity::Ptr entity) {
        if (checkComponents(entity)) {
            getEntities().push_back(entity);
            return true;
        }
        return false;
    }

    bool System::removeEntity(entities::Entity::Ptr entity) {
        auto itr = std::find(getEntities().begin(), getEntities().end(), entity);
        if (itr == getEntities().end()) {
            return false;
        }

        std::size_t size = getEntities().size();
        getEntities().erase(itr);
        return size > getEntities().size();
    }

    System::Ptr System::createInstance(const std::string& systemName, SystemManager& manager) {
        Ptr ptr = RegisteredSystems::construct(systemName, manager);

        if (!ptr) { // We create an external system
            ExternSystem::Ptr e = std::make_shared<ExternSystem>(manager, systemName);
            return e;
        }

        return ptr;
    }

    SystemManager& System::getSystemManager() {
        return m_manager;
    }

    std::vector<entities::Entity::Ptr>& System::getEntities() {
        return m_entities;
    }

    events::StoreEventConnections& System::getStoreEventConnections() {
        return m_storeEventConnections;
    }

    const events::StoreEventConnections& System::getStoreEventConnections() const {
        return m_storeEventConnections;
    }

    events::EventConnection System::registerCallback(const std::string &eventType, const events::EventHandler::callbackSignature &callback, const std::string &state) {
        return getStoreEventConnections().registerCallback(eventType, callback, state);
    }
}