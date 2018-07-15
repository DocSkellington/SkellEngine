#include "systems/ExternSystem.h"

#include "systems/SystemManager.h"
#include "Context.h"

namespace engine::systems {
    ExternSystem::ExternSystem(SystemManager &manager) :
        System(manager) {
        m_lua.open_libraries(sol::lib::base, sol::lib::package);
    }

    ExternSystem::~ExternSystem() {
    }

    void ExternSystem::loadLua(const std::string &systemName) {
        m_lua.do_file(getSystemManager().getContext().fileManager->getSystemPath(systemName));
    }

    bool ExternSystem::update(sf::Int64 deltatime, sf::View &view) {
        m_lua["update"](deltatime);
    }

    bool ExternSystem::checkComponents(entities::Entity::Ptr) const {

    }
}