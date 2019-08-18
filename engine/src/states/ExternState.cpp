#include "SkellEngine/states/ExternState.h"

#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/utilities/lua_sfml.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/Context.h"
#include "SkellEngine/entities/components/ExternComponent.h"
#include "SkellEngine/events/ExternEvent.h"
#include "SkellEngine/entities/EntityManager.h"
#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/levels/Level.h"
#include "SkellEngine/input/InputHandler.h"
#include "SkellEngine/utilities/lua_gui.h"

namespace engine::states {
    ExternState::ExternState(StateManager &manager, const std::string &stateName) :
        State(manager, stateName) {
        luaFunctions();

        sol::safe_function_result load = m_lua.safe_script_file(getStateContext().context.fileManager->getStatePath(stateName));

        if (!load.valid()) {
            sol::error e = load;
            throw e;
        }
    }
     
    ExternState::~ExternState() {
        getStoreEventConnections().clearEventConnections();
    }

    void ExternState::onCreate() {
        sol::protected_function func = m_lua["onCreate"];
        if (func) {
            sol::protected_function_result res = func();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the onCreate function", e);
            }
        }
    }

    void ExternState::onDestroy() {
        sol::protected_function func = m_lua["onDestroy"];
        if (func) {
            sol::protected_function_result res = func();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the onDestroy function", e);
            }
        }
    }

    void ExternState::activate() {
        sol::protected_function func = m_lua["activate"];
        if (func) {
            sol::protected_function_result res = func();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the activate function", e);
            }
        }
    }

    void ExternState::deactivate() {
        sol::protected_function func = m_lua["deactivate"];
        if (func) {
            sol::protected_function_result res = func();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the deactivate function", e);
            }
        }
    }

    void ExternState::update(sf::Int64 deltatime) {
        sol::protected_function func = m_lua["update"];
        if (func) {
            sol::protected_function_result res = func(deltatime);
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the update function", e);
            }
        }
    }

    void ExternState::handleEvent(const sf::Event &event) {
        sol::protected_function func = m_lua["handleEvent"];
        if (func) {
            sol::protected_function_result res = func(event);
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the handleEvent function", e);
            }
        }
        else {
            State::handleEvent(event);
        }
    }

    void ExternState::draw(sf::RenderTarget &target) {
        sol::protected_function func = m_lua["draw"];
        if (func) {
            sol::protected_function_result res = func(target);
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternState: " + getStateContext().stateName + ": error during the draw function", e);
            }
        }
    }

    void ExternState::luaFunctions() {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math);

        m_lua.create_named_table("game");

        utilities::MemberStorage::luaFunctions(m_lua);
        entities::components::Component::luaFunctions(m_lua);
        entities::components::ExternComponent::luaFunctions(m_lua);
        entities::Entity::luaFunctions(m_lua);
        events::Event::luaFunctions(m_lua);
        events::ExternEvent::luaFunctions(m_lua);
        getStateContext().entityManager->luaFunctions(m_lua);
        getStateContext().systemManager->luaFunctions(m_lua);
        getStateContext().context.eventHandler->luaFunctions(m_lua);
        getStateContext().context.stateManager->luaFunctions(m_lua);
        getStateContext().level->luaFunctions(m_lua);
        getStateContext().context.inputHandler->luaFunctions(m_lua);
        getStateContext().context.luaFunctions(m_lua);
        
        getStoreEventConnections().luaFunctions(m_lua);

        utilities::registerSFMLLuaFunctions(m_lua);
        utilities::registerTGUILuaFunctions(m_lua, getStateContext().gui);

        m_lua.set_function("setIsTransparent", [this] (bool is) { this->setIsTransparent(is); });
        m_lua.set_function("setIsTranscendant", [this] (bool is) { this->setIsTranscendant(is); });
    }
}