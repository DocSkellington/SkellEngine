#include "systems/SystemManager.h"

namespace engine::systems {
    SystemManager::SystemManager(Context& context) :
        m_context(context) {

    }

    SystemManager::~SystemManager() {

    }

    void SystemManager::update(float deltatime) {
        for (auto& system : m_systems)
            system.second->update(deltatime);
    }
    
    void SystemManager::draw(sf::RenderWindow *window, unsigned int layer) {
        // TODO
    }

    System::Ptr SystemManager::getSystem(const std::string &name) {
        auto system = m_systems.find(name);
        if (system == m_systems.end())
            return System::Ptr(nullptr);
        return system->second;
    }

    bool SystemManager::addSystem(const std::string &name) {
        if (m_systems.find(name) != m_systems.end()) {
            std::cerr << "Error: impossible to use two systems of the same type (" << name << ")\n";
            return false;
        }

        System::Ptr system = System::createInstance(name, *this);
        m_systems.insert(std::make_pair(name, system));
        return true;
    }

    bool SystemManager::loadSystems(const nlohmann::json &systems) {
        bool all = true;
        for (auto &system : systems) {
            if (!addSystem(system.get<std::string>()))
                all = false;
        }
        return all;
    }

    bool SystemManager::removeSystem(const std::string &name) {
        return (m_systems.erase(name) > 0);
    }

    void SystemManager::clear() {
        m_systems.clear();
    }

    bool SystemManager::addEntity(entities::Entity::Ptr entity) {
        bool atLeastOne = false;
        for (auto &system : m_systems) {
            if (system.second->addEntity(entity))
                atLeastOne = true;
        }
        return atLeastOne;
    }

    bool SystemManager::removeEntity(entities::Entity::Ptr entity) {
        bool atLeastOne = false;
        for (auto &system : m_systems) {
            if (system.second->removeEntity(entity))
                atLeastOne = true;
        }
        return atLeastOne;
    }

    Context &SystemManager::getContext() const {
        return m_context;
    }
}