#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <sol.hpp>

#include "states/StateManager.h"
#include "FileManager.h"

namespace engine {
    /**
    * \brief Defines the context of the engine.
    */
    struct Context {
        Context() {}
        Context(const Context&) = delete;

        std::shared_ptr<FileManager> fileManager;

        std::shared_ptr<sol::state> lua;

        std::shared_ptr<sf::RenderWindow> window;

        std::shared_ptr<states::StateManager> stateManager;
    };
}