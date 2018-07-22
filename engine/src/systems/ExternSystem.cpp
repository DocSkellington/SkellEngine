#include "systems/ExternSystem.h"

#include "systems/SystemManager.h"
#include "Context.h"
#include "entities/components/ExternComponent.h"

namespace engine::systems {
    ExternSystem::ExternSystem(SystemManager &manager) :
        System(manager) {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math);
        entities::components::Component::luaFunctions(m_lua);
        entities::Entity::luaFunctions(m_lua);
        manager.getContext().entityManager->luaFunctions(m_lua);
    }

    ExternSystem::~ExternSystem() {
    }

    void ExternSystem::loadLua(const std::string &systemName) {
        m_lua.do_file(getSystemManager().getContext().fileManager->getSystemPath(systemName));
        m_lua["init"]();
    }

    bool ExternSystem::update(sf::Int64 deltatime, sf::View &view) {
        m_lua["update"](deltatime, view);
    }

    bool ExternSystem::checkComponents(entities::Entity::Ptr) const {

    }
}