#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <Thor/Resources.hpp>
#include <sol.hpp>
#include <tmx/MapLoader.hpp>

#include "states/StateManager.h"
#include "entities/EntityManager.h"
#include "systems/SystemManager.h"
#include "files/FileManager.h"

#define LOG_OUTPUT_CONSOLE


namespace engine {
    /**
    * \brief Defines the context of the engine.
    */
    struct Context {
        Context() {}
        Context(const Context&) = delete;

        std::shared_ptr<files::FileManager> fileManager;

        std::shared_ptr<sol::state> lua;

        std::shared_ptr<sf::RenderWindow> window;

        std::shared_ptr<tmx::MapLoader> mapLoader;

        std::shared_ptr<thor::ResourceHolder<sf::Texture, std::string>> textureHolder;

        std::shared_ptr<states::StateManager> stateManager;

        std::shared_ptr<entities::EntityManager> entityManager;

        std::shared_ptr<systems::SystemManager> systemManager;
    };
}