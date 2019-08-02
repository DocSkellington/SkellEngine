#include "SkellEngine/systems/System.h"

#include <cctype>

#include <SkellEngine/tmxlite/Log.hpp>
#include "SkellEngine/systems/ExternSystem.h"
#include "SkellEngine/errors/ConstructorNotValid.h"

namespace engine::systems {
    System::System(SystemManager& manager) :
        m_manager(manager) {
        
    }

    System::~System() {

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
            ExternSystem::Ptr e = std::make_shared<ExternSystem>(manager);
            e->loadLua(systemName);
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
}