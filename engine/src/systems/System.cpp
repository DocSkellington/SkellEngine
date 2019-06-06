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
        auto systemConstructor = getMapToSystem()->find(systemName);

        if (systemConstructor == getMapToSystem()->end()) {
            ExternSystem::Ptr e = std::make_shared<ExternSystem>(manager);
            e->loadLua(systemName);
            return e;
        }
        else if (!systemConstructor->second) {
            throw errors::ConstructorNotValid(systemName + " is a known system but can not be constructed. It's probably an engine bug.");
        }
        else {
            try {
                auto system = systemConstructor->second(manager);
                return system;
            }
            catch (const std::bad_function_call &e) {
                tmx::Logger::logError("Error while constructing the system " + systemName, e);
            }
        }
        return nullptr;
    }

    std::shared_ptr<System::MapType> System::getMapToSystem() {
        static std::shared_ptr<MapType> map = std::make_shared<MapType>();
        return map;
    }

    SystemManager& System::getSystemManager() {
        return m_manager;
    }

    std::vector<entities::Entity::Ptr>& System::getEntities() {
        return m_entities;
    }
}