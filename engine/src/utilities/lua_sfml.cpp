#include "SkellEngine/utilities/lua_sfml.h"

namespace engine::utilities {
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
}