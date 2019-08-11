#include "SkellEngine/systems/ExternSystem.h"

#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/Context.h"
#include "SkellEngine/entities/components/ExternComponent.h"
#include "SkellEngine/events/ExternEvent.h"
#include "SkellEngine/entities/EntityManager.h"
#include "SkellEngine/files/FileManager.h"

// T: type of the data in the vector (float, unsigned int and so on)
template <typename T>
void registerVector2(sol::state &lua, const std::string &name) {
    lua.new_usertype<sf::Vector2<T>>(name,
        sol::constructors<
            sf::Vector2<T>(),
            sf::Vector2<T>(T, T),
            sf::Vector2<T>(const sf::Vector2<int>&),
            sf::Vector2<T>(const sf::Vector2<long>&),
            sf::Vector2<T>(const sf::Vector2<float>&),
            sf::Vector2<T>(const sf::Vector2<double>&),
            sf::Vector2<T>(const sf::Vector2<bool>&)
        >(),

        "x", &sf::Vector2<T>::x,
        "y", &sf::Vector2<T>::y,

        sol::meta_function::unary_minus, sol::resolve<sf::Vector2<T>(const sf::Vector2<T>&)>(sf::operator-),
        sol::meta_function::addition, sol::resolve<sf::Vector2<T>(const sf::Vector2<T>&, const sf::Vector2<T>&)>(sf::operator+),
        sol::meta_function::subtraction, sol::resolve<sf::Vector2<T>(const sf::Vector2<T>&, const sf::Vector2<T>&)>(sf::operator-),
        sol::meta_function::multiplication, sol::overload(
            sol::resolve<sf::Vector2<T>(const sf::Vector2<T>&, T)>(sf::operator*),
            sol::resolve<sf::Vector2<T>(T, const sf::Vector2<T>&)>(sf::operator*)
        ),
        sol::meta_function::division, sol::resolve<sf::Vector2<T>(const sf::Vector2<T>&, T)>(sf::operator/),
        sol::meta_function::equal_to, sol::resolve<bool(const sf::Vector2<T>&, const sf::Vector2<T>&)>(sf::operator==)
    );
}

template <typename T>
void registerRect(sol::state &lua, const std::string &name) {
    lua.new_usertype<sf::Rect<T>>(name,
        sol::constructors<
            sf::Rect<T>(),
            sf::Rect<T>(T, T, T, T),
            sf::Rect<T>(const sf::Vector2<T>&, const sf::Vector2<T>&),
            sf::Rect<T>(const sf::Rect<int>&),
            sf::Rect<T>(const sf::Rect<long>&),
            sf::Rect<T>(const sf::Rect<float>&),
            sf::Rect<T>(const sf::Rect<double>&)
        >(),

        "left", &sf::Rect<T>::left,
        "top", &sf::Rect<T>::top,
        "width", &sf::Rect<T>::width,
        "height", &sf::Rect<T>::height,

        "contains", sol::overload(
            sol::resolve<bool(T, T) const>(&sf::Rect<T>::contains),
            sol::resolve<bool(const sf::Vector2<T>&) const>(&sf::Rect<T>::contains)
        ),
        "intersects", sol::overload(
            sol::resolve<bool(const sf::Rect<T>&) const>(&sf::Rect<T>::intersects),
            sol::resolve<bool(const sf::Rect<T>&, sf::Rect<T>&) const>(&sf::Rect<T>::intersects)
        ),

        sol::meta_function::equal_to, sol::resolve<bool(const sf::Rect<T>&, const sf::Rect<T>&)>(sf::operator==)
    );
}

void registerSFMLLuaFunctions(sol::state &lua) {
    registerVector2<int>(lua, "Vector2i");
    registerVector2<long>(lua, "Vector2l");
    registerVector2<float>(lua, "Vector2f");
    registerVector2<double>(lua, "Vector2d");
    registerVector2<bool>(lua, "Vector2b");

    registerRect<int>(lua, "IntRect");
    registerRect<long>(lua, "LongRect");
    registerRect<float>(lua, "FloatRect");
    registerRect<double>(lua, "DoubleRect");

    lua.new_usertype<sf::View>("view",
        sol::constructors<
            sf::View(),
            sf::View(const sf::FloatRect&),
            sf::View(const sf::Vector2f&, const sf::Vector2f&)
        >(),

        "setCenter", sol::overload(
            sol::resolve<void(float, float)>(&sf::View::setCenter),
            sol::resolve<void(const sf::Vector2f&)>(&sf::View::setCenter)
        ),
        "setSize", sol::overload(
            sol::resolve<void(float, float)>(&sf::View::setSize),
            sol::resolve<void(const sf::Vector2f&)>(&sf::View::setSize)
        ),
        "setRotation", &sf::View::setRotation,
        "setViewport", &sf::View::setViewport,
        "reset", &sf::View::reset,
        "getCenter", &sf::View::getCenter,
        "getSize", &sf::View::getSize,
        "getRotation", &sf::View::getRotation,
        "getViewport", &sf::View::getViewport,
        "move", sol::overload(
            sol::resolve<void(float, float)>(&sf::View::move),
            sol::resolve<void(const sf::Vector2f&)>(&sf::View::move)
        ),
        "rotate", &sf::View::rotate,
        "zoom", &sf::View::zoom
    );
}

namespace engine::systems {
    ExternSystem::ExternSystem(SystemManager &manager, const std::string &systemName) :
        System(manager) {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math);

        m_lua.create_named_table("game");

        registerSFMLLuaFunctions(m_lua);

        utilities::MemberStorage::luaFunctions(m_lua);
        entities::components::Component::luaFunctions(m_lua);
        entities::components::ExternComponent::luaFunctions(m_lua);
        entities::Entity::luaFunctions(m_lua);
        events::Event::luaFunctions(m_lua);
        events::ExternEvent::luaFunctions(m_lua);
        manager.getContext().entityManager->luaFunctions(m_lua);
        manager.getContext().systemManager->luaFunctions(m_lua);
        manager.getContext().context.eventHandler->luaFunctions(m_lua);
        
        getStoreEventConnections().luaFunctions(m_lua);

        loadLua(systemName);
    }

    ExternSystem::~ExternSystem() {
        sol::protected_function destroyFunc = m_lua["destroy"];
        if (destroyFunc) {
            sol::protected_function_result res = destroyFunc();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternSystem: error during destroy. For more details: ", e);
            }
        }
        getStoreEventConnections().clearEventConnections();
    }

    void ExternSystem::loadLua(const std::string &systemName) {
        m_systemName = systemName;

        sol::protected_function_result load = m_lua.safe_script_file(getSystemManager().getContext().context.fileManager->getSystemPath(systemName));

        if (!load.valid()) {
            sol::error e = load;
            tmx::Logger::logError("ExternSystem: " + m_systemName + ": impossible to load the file of the system " + systemName, e);
        }
        else {
            sol::protected_function initFunc = m_lua["init"];
            if (initFunc) {
                sol::protected_function_result res = initFunc();
                if (!res.valid()) {
                    sol::error e = res;
                    tmx::Logger::logError("ExternSystem: " + m_systemName + ": error during init. The system could not be properly initiliazed and may not work as intented", e);
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
                tmx::Logger::logError("ExternSystem: " + m_systemName + ": error during update", e);
                return false;
            }
        }
        else {
            tmx::Logger::log("ExternSystem: " + m_systemName + " does not have an update function", tmx::Logger::Type::Warning);
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
                    tmx::Logger::log("ExternSystem: " + m_systemName + ": checkComponents must return a boolean.", tmx::Logger::Type::Error);
                }
            }
            else {
                sol::error e = res;
                tmx::Logger::logError("ExternSystem: " + m_systemName + ": error during checkComponents. No entities will be added in the system", e);
            }
        }
        else {
            tmx::Logger::log("ExternSystem: " + m_systemName + " does not have a checkComponents function. Therefore, no entities will be added in the system.", tmx::Logger::Type::Warning);
        }
        return false;
    }
}