#pragma once

#include <string>

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>

namespace engine::utilities {
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

    void registerSFMLLuaFunctions(sol::state &lua);
}