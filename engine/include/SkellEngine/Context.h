#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <sol/sol.hpp>
#include <TGUI/TGUI.hpp>

#include "SkellEngine/files/GameDescription.h"
#include "SkellEngine/Logger.h"

namespace engine {
    namespace files {
        class FileManager;
    }

    namespace states {
        class StateManager;
    }

    namespace input {
        class InputHandler;
    }

    namespace events {
        class EventHandler;
    }

    /**
    * \brief Defines the context of the engine.
    */
    class Context {
    public:
        /**
         * \brief Constructor
         * \param baseMediaPath The path to the "media" folder
         * \param graphical Whether to create the window
         */
        Context(const std::filesystem::path &baseMediaPath, bool graphical = true);
        Context(const Context&) = delete;

        /**
         * \brief Closes the engine
         * 
         * Actually, it closes the window. The engine will shutdown itself at the next loop iteration
         */
        void closeEngine();

        /**
         * \brief Registers Lua functions
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    public:
        /**
         * \brief The logger
         */
        Logger logger;
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
         * \brief The state manager
         */
        std::shared_ptr<states::StateManager> stateManager;

        /**
         * \brief The event handler
         */
        std::shared_ptr<events::EventHandler> eventHandler;

        /**
         * \brief The input handler
         */
        std::shared_ptr<input::InputHandler> inputHandler;

    private:
        void createWindow(const files::GameDescription::WindowDescription &windowDescription, const std::string &version);
    };
}