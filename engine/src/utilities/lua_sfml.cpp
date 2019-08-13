#include "SkellEngine/utilities/lua_sfml.h"

namespace engine::utilities {
    void registerSFMLLuaFunctions(sol::state &lua) {
        registerVector2<int>(lua, "Vector2i");
        registerVector2<unsigned int>(lua, "Vector2u");
        registerVector2<long>(lua, "Vector2l");
        registerVector2<unsigned long>(lua, "Vector2ul");
        registerVector2<float>(lua, "Vector2f");
        registerVector2<double>(lua, "Vector2d");
        registerVector2<bool>(lua, "Vector2b");

        registerVector3<int>(lua, "Vector3i");
        registerVector3<unsigned int>(lua, "Vector3u");
        registerVector3<long>(lua, "Vector3l");
        registerVector3<unsigned long>(lua, "Vector3ul");
        registerVector3<float>(lua, "Vector3f");
        registerVector3<double>(lua, "Vector3d");
        registerVector3<bool>(lua, "Vector3b");

        registerRect<int>(lua, "IntRect");
        registerRect<unsigned int>(lua, "UnsignedIntRect");
        registerRect<long>(lua, "LongRect");
        registerRect<unsigned long>(lua, "UnsignedLongRect");
        registerRect<float>(lua, "FloatRect");
        registerRect<double>(lua, "DoubleRect");

        lua.new_usertype<sf::View>("View",
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

        lua.new_usertype<sf::Color>("Color",
            sol::constructors<
                sf::Color(),
                sf::Color(sf::Uint8, sf::Uint8, sf::Uint8),
                sf::Color(sf::Uint8, sf::Uint8, sf::Uint8, sf::Uint8),
                sf::Color(sf::Uint32)
            >(),
            "toInteger", &sf::Color::toInteger,
            "r", &sf::Color::r,
            "g", &sf::Color::g,
            "b", &sf::Color::b,
            "a", &sf::Color::a,
            sol::meta_function::addition, sol::resolve<sf::Color(const sf::Color&, const sf::Color&)>(sf::operator+),
            sol::meta_function::subtraction, sol::resolve<sf::Color(const sf::Color&, const sf::Color&)>(sf::operator-),
            sol::meta_function::multiplication, sol::resolve<sf::Color(const sf::Color&, const sf::Color&)>(sf::operator*),
            // Some witchcraft to make sure the user can not modify the static const variables
            "Black", sol::readonly_property([]() { return sf::Color::Black; }),
            "White", sol::readonly_property([]() { return sf::Color::White; }),
            "Red", sol::readonly_property([]() { return sf::Color::Red; }),
            "Green", sol::readonly_property([]() { return sf::Color::Green; }),
            "Blue", sol::readonly_property([]() { return sf::Color::Blue; }),
            "Yellow", sol::readonly_property([]() { return sf::Color::Yellow; }),
            "Magenta", sol::readonly_property([]() { return sf::Color::Magenta; }),
            "Cyan", sol::readonly_property([]() { return sf::Color::Cyan; }),
            "Transparent", sol::readonly_property([]() { return sf::Color::Transparent; })
        );

        lua.new_usertype<sf::RenderTarget>("RenderTarget",
            sol::no_constructor,
            "clear", &sf::RenderTarget::clear,
            "setView", &sf::RenderTarget::setView,
            "getView", &sf::RenderTarget::getView,
            "getDefaultView", &sf::RenderTarget::getDefaultView,
            "getViewport", &sf::RenderTarget::getViewport,
            "mapPixelToCoords", sol::overload(
                sol::resolve<sf::Vector2f(const sf::Vector2i&)const>(&sf::RenderTarget::mapPixelToCoords),
                sol::resolve<sf::Vector2f(const sf::Vector2i&, const sf::View&)const>(&sf::RenderTarget::mapPixelToCoords)
            ),
            "mapCoordsToPixel", sol::overload(
                sol::resolve<sf::Vector2i(const sf::Vector2f&)const>(&sf::RenderTarget::mapCoordsToPixel),
                sol::resolve<sf::Vector2i(const sf::Vector2f&, const sf::View&)const>(&sf::RenderTarget::mapCoordsToPixel)
            ),
            "getSize", &sf::RenderTarget::getSize
        );

        /** \todo TODO: more support when Unicode is correctly handled in the engine */
        lua.new_usertype<sf::String>("String",
            sol::constructors<
                sf::String(),
                sf::String(char),
                sf::String(wchar_t),
                sf::String(sf::Uint32),
                sf::String(const char*),
                sf::String(const std::string&),
                sf::String(const wchar_t*),
                sf::String(const std::wstring&),
                sf::String(const sf::Uint32*),
                sf::String(const std::basic_string<sf::Uint32>&),
                sf::String(const sf::String&)
            >(),
            sol::meta_function::to_string, &sf::String::operator std::__cxx11::string,
            "clear", &sf::String::clear,
            "getSize", &sf::String::getSize,
            "isEmpty", &sf::String::isEmpty
        );

        lua.new_usertype<sf::Clipboard>("Clipboard",
            sol::no_constructor,
            "getString", &sf::Clipboard::getString,
            "setString", &sf::Clipboard::setString
        );
    }
}