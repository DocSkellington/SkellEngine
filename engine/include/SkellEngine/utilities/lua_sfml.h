#pragma once

#include <string>

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>

namespace engine::utilities {
    /**
     * \brief Registers Lua functions for Vector2 types
     * \param lua The Lua state
     * \param name The name under which the Vector2<T> must be registered
     * \tparam T The type of the value in the Vector2
     */
    template <typename T>
    void registerVector2(sol::state &lua, const std::string &name) {
        lua.new_usertype<sf::Vector2<T>>(name,
            sol::constructors<
                sf::Vector2<T>(),
                sf::Vector2<T>(T, T),
                sf::Vector2<T>(const sf::Vector2<int>&),
                sf::Vector2<T>(const sf::Vector2<unsigned int>&),
                sf::Vector2<T>(const sf::Vector2<long>&),
                sf::Vector2<T>(const sf::Vector2<unsigned long>&),
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

    /**
     * \brief Registers Lua functions for Vector3 types
     * \param lua The Lua state
     * \param name The name under which the Vector3<T> must be registered
     * \tparam T The type of the value in the Vector3
     */
    template <typename T>
    void registerVector3(sol::state &lua, const std::string &name) {
        lua.new_usertype<sf::Vector3<T>>(name,
            sol::constructors<
                sf::Vector3<T>(),
                sf::Vector3<T>(T, T, T),
                sf::Vector3<T>(const sf::Vector3<int>&),
                sf::Vector3<T>(const sf::Vector3<unsigned int>&),
                sf::Vector3<T>(const sf::Vector3<long>&),
                sf::Vector3<T>(const sf::Vector3<unsigned long>&),
                sf::Vector3<T>(const sf::Vector3<float>&),
                sf::Vector3<T>(const sf::Vector3<double>&),
                sf::Vector3<T>(const sf::Vector3<bool>&)
            >(),

            "x", &sf::Vector3<T>::x,
            "y", &sf::Vector3<T>::y,
            "z", &sf::Vector3<T>::z,

            sol::meta_function::unary_minus, sol::resolve<sf::Vector3<T>(const sf::Vector3<T>&)>(sf::operator-),
            sol::meta_function::addition, sol::resolve<sf::Vector3<T>(const sf::Vector3<T>&, const sf::Vector3<T>&)>(sf::operator+),
            sol::meta_function::subtraction, sol::resolve<sf::Vector3<T>(const sf::Vector3<T>&, const sf::Vector3<T>&)>(sf::operator-),
            sol::meta_function::multiplication, sol::overload(
                sol::resolve<sf::Vector3<T>(const sf::Vector3<T>&, T)>(sf::operator*),
                sol::resolve<sf::Vector3<T>(T, const sf::Vector3<T>&)>(sf::operator*)
            ),
            sol::meta_function::division, sol::resolve<sf::Vector3<T>(const sf::Vector3<T>&, T)>(sf::operator/),
            sol::meta_function::equal_to, sol::resolve<bool(const sf::Vector3<T>&, const sf::Vector3<T>&)>(sf::operator==)
        );
    }

    /**
     * \brief Registers Lua functions for Rect types
     * \param lua The Lua state
     * \param name The name under which the Rect<T> must be registered
     * \tparam T The type of the value in the Rect
     */
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

    /**
     * \brief Registers Lua functions for SFML types
     * \param lua The Lua state
     */
    void registerSFMLLuaFunctions(sol::state &lua);
}