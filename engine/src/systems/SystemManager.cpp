#include "SkellEngine/systems/SystemManager.h"

#include <SkellEngine/tmxlite/Log.hpp>
#include "SkellEngine/Context.h"

namespace engine::systems {
    SystemManager::SystemManager(states::StateContext& context) :
        m_context(context) {
    }

    void SystemManager::update(sf::Int64 deltatime) {
        for (auto& system : m_systems) {
            system.second->update(deltatime, m_view);
        }
    }
    
    void SystemManager::draw(sf::RenderWindow &window, unsigned int layer) {
        getContext().context.window->setView(m_view);
        for (auto &system : m_graphicalSystems)
            system.second->draw(window, layer, m_view);
    }

    System::Ptr SystemManager::getSystem(std::string name) {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        auto system = m_systems.find(name);
        if (system == m_systems.end())
            return System::Ptr(nullptr);
        return system->second;
    }

    bool SystemManager::addSystem(std::string name) {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (m_systems.find(name) != m_systems.end()) {
            tmx::Logger::log("Impossible to use two systems of the same type (" + name + ").", tmx::Logger::Type::Error);
            return false;
        }

        System::Ptr system = System::createInstance(name, *this);
        if (name.substr(0, std::string("graphical").length()) == "graphical") {
            // If the name starts with Graphical, we register it as a GraphicalSystem
            m_graphicalSystems.insert(std::make_pair(name, std::static_pointer_cast<GraphicalSystem>(system)));
        }
        m_systems.insert(std::make_pair(name, system));
        return true;
    }

    bool SystemManager::loadSystems(const std::vector<std::string> &systems) {
        clear();
        for (auto &system : systems) {
            if (!addSystem(system)) {
                clear();
                return false;    
            }
        }
        return true;
    }

    bool SystemManager::removeSystem(const std::string &name) {
        bool graph = true;
        if (name.substr(0, std::string("graphical").length()) == "graphical") {
            graph = m_graphicalSystems.erase(name) > 0;
        }
        return (graph && m_systems.erase(name) > 0);
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

    states::StateContext &SystemManager::getContext() const {
        return m_context;
    }

    void SystemManager::luaFunctions(sol::state &lua) const {
        lua.new_usertype<SystemManager>("SystemManager",
            "addEntity", &SystemManager::addEntity,
            "removeEntity", &SystemManager::removeEntity
        );

        lua["game"]["systemManager"] = this;
    }
}