#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <Thor/Resources.hpp>
#include <sol/sol.hpp>
#include <SkellEngine/tmxlite/Map.hpp>
#include <TGUI/TGUI.hpp>

#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/entities/EntityManager.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/map/Map.h"

namespace engine {
    /**
    * \brief Defines the context of the engine.
    * \ingroup Engine
    */
    struct Context {
        Context() {}
        Context(const Context&) = delete;

        std::shared_ptr<files::FileManager> fileManager;

        std::shared_ptr<sol::state> lua;

        std::shared_ptr<sf::RenderWindow> window;

        std::shared_ptr<tgui::Gui> gui;

        std::shared_ptr<map::Map> map;

        std::shared_ptr<thor::ResourceHolder<sf::Texture, std::string>> textureHolder;
        
        std::shared_ptr<thor::ResourceHolder<sf::Font, std::string>> fontHolder;

        std::shared_ptr<states::StateManager> stateManager;

        std::shared_ptr<entities::EntityManager> entityManager;

        std::shared_ptr<systems::SystemManager> systemManager;
    };
}