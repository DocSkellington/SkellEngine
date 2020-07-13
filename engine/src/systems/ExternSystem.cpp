#include "SkellEngine/systems/ExternSystem.hpp"

#include "SkellEngine/systems/SystemManager.hpp"
#include "SkellEngine/Context.hpp"
#include "SkellEngine/entities/components/ExternComponent.hpp"
#include "SkellEngine/events/ExternEvent.hpp"
#include "SkellEngine/entities/EntityManager.hpp"
#include "SkellEngine/files/FileManager.hpp"
#include "SkellEngine/utilities/lua_sfml.hpp"
#include "SkellEngine/states/StateManager.hpp"
#include "SkellEngine/levels/Level.hpp"
#include "SkellEngine/input/InputHandler.hpp"
#include "SkellEngine/utilities/lua_gui.hpp"

namespace engine::systems {
    ExternSystem::ExternSystem(SystemManager &manager, const std::string &systemName) :
        System(manager) {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math);

        m_lua.create_named_table("game");

        utilities::registerSFMLLuaFunctions(m_lua);
        // TODO: utilities::registerTGUILuaFunctions(m_lua, manager.getContext().gui);

        utilities::VariableStorage::luaFunctions(m_lua);
        utilities::VariableStorage::luaFunctions(m_lua);
        entities::components::Component::luaFunctions(m_lua);
        entities::components::ExternComponent::luaFunctions(m_lua);
        entities::Entity::luaFunctions(m_lua);
        events::Event::luaFunctions(m_lua);
        events::ExternEvent::luaFunctions(m_lua);
        manager.getContext().entityManager->luaFunctions(m_lua);
        manager.getContext().systemManager->luaFunctions(m_lua);
        manager.getContext().context.eventHandler->luaFunctions(m_lua);
        manager.getContext().context.stateManager->luaFunctions(m_lua);
        manager.getContext().level->luaFunctions(m_lua);
        manager.getContext().context.inputHandler->luaFunctions(m_lua);
        manager.getContext().context.luaFunctions(m_lua);
        manager.getContext().context.fileManager->luaFunctions(m_lua);
        
        getStoreEventConnections().luaFunctions(m_lua);

        loadLua(systemName);
    }

    ExternSystem::~ExternSystem() {
        sol::protected_function destroyFunc = m_lua["destroy"];
        if (destroyFunc) {
            sol::protected_function_result res = destroyFunc();
            if (!res.valid()) {
                sol::error e = res;
                getSystemManager().getContext().context.logger.logError("ExternSystem: error during destroy. For more details: ", e);
            }
        }
        getStoreEventConnections().clearEventConnections();
    }

    void ExternSystem::loadLua(const std::string &systemName) {
        m_systemName = systemName;

        sol::protected_function_result load = m_lua.safe_script_file(getSystemManager().getContext().context.fileManager->getSystemPath(systemName));

        if (!load.valid()) {
            sol::error e = load;
            getSystemManager().getContext().context.logger.logError("ExternState: impossible to load the Lua script for system " + systemName, e);
        }
        else {
            sol::protected_function initFunc = m_lua["init"];
            if (initFunc) {
                sol::protected_function_result res = initFunc();
                if (!res.valid()) {
                    sol::error e = res;
                    getSystemManager().getContext().context.logger.logError("ExternSystem: " + m_systemName + ": error during init. The system could not be properly initiliazed and may not work as intended", e);
                }
            }
        }
    }

    bool ExternSystem::update(sf::Int64 deltatime, sf::View &view) {
        sol::protected_function updateFunc = m_lua["update"];
        if (updateFunc) {
            sol::protected_function_result res = updateFunc(deltatime, view);
            if (!res.valid()) {
                sol::error e = res;
                getSystemManager().getContext().context.logger.logError("ExternSystem: " + m_systemName + ": error during update", e);
                return false;
            }
        }
        else {
            getSystemManager().getContext().context.logger.log("ExternSystem: " + m_systemName + " does not have an update function", LogType::Warning);
        }
        return true;
    }

    bool ExternSystem::checkComponents(entities::Entity::Ptr entity) const {
        sol::protected_function checkFunc = m_lua["checkComponents"];
        if (checkFunc) {
            sol::protected_function_result res = checkFunc(entity);
            if (res.valid()) {
                if (res.get_type() == sol::type::boolean) {
                    bool r = res;
                    return r;
                }
                else {
                    getSystemManager().getContext().context.logger.log("ExternSystem: " + m_systemName + ": checkComponents must return a boolean.", LogType::Error);
                }
            }
            else {
                sol::error e = res;
                getSystemManager().getContext().context.logger.logError("ExternSystem: " + m_systemName + ": error during checkComponents. No entities will be added in the system", e);
            }
        }
        else {
            getSystemManager().getContext().context.logger.log("ExternSystem: " + m_systemName + " does not have a checkComponents function. Therefore, no entities will be added in the system.", LogType::Warning);
        }
        return false;
    }
}