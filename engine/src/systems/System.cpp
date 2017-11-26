#include "systems/System.h"

#include <cctype>

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
            std::cerr << "Error: impossible to remove an entity not used in a system\n";
            return false;
        }

        auto itrErase = getEntities().erase(itr);
        return !(itr == itrErase);
    }

    System::Ptr System::createInstance(const std::string& systemName, SystemManager& manager) {
        std::cout << systemName << '\n';
        auto systemConstructor = getMapToSystem()->find(systemName);

        if (systemConstructor == getMapToSystem()->end()) {
            // TODO: return generic system
        }
        else if (!systemConstructor->second) {
            std::cerr << "SYSTEMCONSTRUCTOR\n";
        }
        else {
            try {
                auto system = systemConstructor->second(manager);
                return system;
            }
            catch (std::bad_function_call e) {
                std::cerr << "Error while constructing the system " << systemName << ":\n";
                std::cerr << e.what() << '\n';
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