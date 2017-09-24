#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "states/StateManager.h"

namespace engine {
    struct Context {

        std::shared_ptr<sf::RenderWindow> window;

        std::shared_ptr<states::StateManager> stateManager;
    };
}