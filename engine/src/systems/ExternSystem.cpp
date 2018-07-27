#include "systems/ExternSystem.h"

#include "systems/SystemManager.h"
#include "Context.h"
#include "entities/components/ExternComponent.h"

// V: type of the vector (sf::Vector2f, sf::Vector2u and so on)
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
    ExternSystem::ExternSystem(SystemManager &manager) :
        System(manager) {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math);

        registerSFMLLuaFunctions(m_lua);

        entities::components::Component::luaFunctions(m_lua);
        entities::Entity::luaFunctions(m_lua);
        manager.getContext().entityManager->luaFunctions(m_lua);
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
    }

    void ExternSystem::loadLua(const std::string &systemName) {
        m_lua.do_file(getSystemManager().getContext().fileManager->getSystemPath(systemName));
        sol::protected_function initFunc = m_lua["init"];
        if (initFunc) {
            sol::protected_function_result res = initFunc();
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternSystem: error during init. The system could not be properly initiliazed and may not work as intented. For more details: ", e);
            }
        }
    }

    bool ExternSystem::update(sf::Int64 deltatime, sf::View &view) {
        sol::protected_function updateFunc = m_lua["update"];
        if (updateFunc) {
            sol::protected_function_result res = updateFunc(deltatime, view);
            if (!res.valid()) {
                sol::error e = res;
                tmx::Logger::logError("ExternSystem: error during update. For more details: ", e);
                return false;
            }
        }
        return true;
    }

    bool ExternSystem::checkComponents(entities::Entity::Ptr entity) const {
        sol::protected_function checkFunc = m_lua["checkComponents"];
        if (checkFunc) {
            sol::protected_function_result res = checkFunc(entity);
            if (res.valid()) {
                bool r = res;
                return r;
            }
            else {
                sol::error e = res;
                tmx::Logger::logError("ExterSystem: error during checkComponents. No entities will be added in the system. For more details: ", e);
            }
        }
        return false;
    }
}