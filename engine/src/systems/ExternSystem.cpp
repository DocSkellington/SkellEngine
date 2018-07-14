#include "systems/ExternSystem.h"

namespace engine::systems {
    ExternSystem::ExternSystem(SystemManager &manager) :
        System(manager) {

    }

    ExternSystem::~ExternSystem() {

    }

    void ExternSystem::setLuaScript(sol::table &lua) {
        m_lua = lua;
    }

    bool ExternSystem::update(sf::Int64 deltatime, sf::View &view) {

    }

    bool ExternSystem::checkComponents(entities::Entity::Ptr) const {

    }
}