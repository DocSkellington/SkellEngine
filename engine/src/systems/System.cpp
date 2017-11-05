#include "systems/System.h"

namespace engine::systems {
    System::MapType System::m_nameToSystem;

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
            std::cerr << "Error: impossible to remove an entity not used in a system\n";
            return false;
        }

        auto itrErase = getEntities().erase(itr);
        return !(itr == itrErase);
    }

    System::Ptr System::createInstance(const std::string &systemName, SystemManager& manager) {
        auto systemConstructor = getMapToSystem().find(systemName);

        if (systemConstructor == getMapToSystem().end()) {
            // TODO: return generic system
        }
        return systemConstructor->second(manager);
    }

    System::MapType& System::getMapToSystem() {
        return m_nameToSystem;
    }

    SystemManager& System::getSystemManager() {
        return m_manager;
    }

    std::vector<entities::Entity::Ptr>& System::getEntities() {
        return m_entities;
    }
}