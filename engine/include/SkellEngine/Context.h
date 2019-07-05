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
#include "SkellEngine/events/EventHandler.h"

namespace engine {
    /**
    * \brief Defines the context of the engine.
    * \ingroup Engine
    */
    struct Context {
        Context() {}
        Context(const Context&) = delete;

        /**
         * \brief The file manager
         */
        std::shared_ptr<files::FileManager> fileManager;

        /**
         * \brief The Lua state
         * 
         * \see The sol documentation
         */
        std::shared_ptr<sol::state> lua;

        /**
         * \brief The SFML window
         */
        std::shared_ptr<sf::RenderWindow> window;

        /**
         * \brief The Graphical User Interface tool
         */
        std::shared_ptr<tgui::Gui> gui;

        /**
         * \brief The current map
         */
        std::shared_ptr<map::Map> map;

        /**
         * \brief The texture holder
         * 
         * Every texture used by the engine should be registered in this holder
         */
        std::shared_ptr<thor::ResourceHolder<sf::Texture, std::string>> textureHolder;
        
        /**
         * \brief The font holder
         * 
         * Every font used by the engine should be registered in this holder
         */
        std::shared_ptr<thor::ResourceHolder<sf::Font, std::string>> fontHolder;

        /**
         * \brief The state manager
         */
        std::shared_ptr<states::StateManager> stateManager;

        /**
         * \brief The entity manager
         */
        std::shared_ptr<entities::EntityManager> entityManager;

        /**
         * \brief The system manager
         */
        std::shared_ptr<systems::SystemManager> systemManager;

        /**
         * \brief The event handler
         */
        std::shared_ptr<events::EventHandler> eventHandler;
    };
}