#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

namespace engine {
    struct Context {

        std::shared_ptr<sf::Window> window;
    };
}