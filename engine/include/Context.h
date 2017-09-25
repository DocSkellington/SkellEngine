#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <sol.hpp>

#include "states/StateManager.h"

namespace engine {
    /**
    * \brief Defines the context of the engine.
    */
    struct Context {
        std::shared_ptr<sol::state> lua;

        std::shared_ptr<sf::RenderWindow> window;

        std::shared_ptr<states::StateManager> stateManager;
    };
}