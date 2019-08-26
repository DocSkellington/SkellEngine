#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "SkellEngine/Logger.h"

namespace engine::files {
    /**
     * \brief The game's parameters.
     * 
     * It should be loaded trough JSON. It contains every information global to the entire game (such as the window's parameters, the media and so on).
     */
    struct GameDescription {
    public:
        /**
         * \brief Describes the logging system.
         * 
         * It should be loaded trough JSON
         */
        struct LogDescription {
            /**
             * \brief Where the logging messages are printed (in nothing, in the standard output, in a file or in both)
             */
            LogOutput output = LogOutput::Console;
            /**
             * \brief The log level.
             * 
             * The log messages with a lower type that the level are not logged.
             */
            LogType level = LogType::Info;
            /**
             * \brief The path to the log file
             */
            std::filesystem::path logPath;
        };

        /**
         * \brief Describes the window.
         * 
         * It should be loaded trough JSON.
         */
        struct WindowDescription {
            /**
             * \brief Whether the window must be fullscreen
             */
            bool fullscreen = false;
            /**
             * \brief Whether to show the titlebar (in windowed mode)
             */
            bool titlebar = true;
            /**
             * \brief Whether to allow the user to resize the window (in windowed mode)
             */
            bool resize = true;
            /**
             * \brief Whether to show the close button (in windowed mode)
             */
            bool close = true;
            /**
             * \brief Whether to show the version of the game in the titlebar (in windowed mode and if the titlebar is shown)
             */
            bool version = true;
            /**
             * \brief Whether to show the name of the engine in the titlebar (in windowed mode and if the titlebar is shown)
             */
            bool engine = true;
            /**
             * \brief The title to set in the titlebar (in windowed mode and if the titlebar is shown)
             */
            std::string title = "";
            /**
             * \brief The width of the window (in windowed mode)
             */
            int width = 800;
            /**
             * \brief The height of the window (in windowed mode)
             */
            int height = 600;
            /**
             * \brief The level of anti-aliasing
             */
            int antialiasingLevel = 0;
            /**
             * \brief The FPS limit
             * 
             * It's ignored if the vertical synchronisation is active
             * 
             * Set to 0 or a negative number if you want an unlimited FPS
             */
            int FPS = 60;
            /**
             * \brief Whether to use the vertical synchronisation
             * 
             * If true, the FPS limit is ignored
             */
            bool verticalSynchronisation = false;
        };

        /**
         * \brief Describes the media folder.
         * 
         * It should be loaded trough JSON.
         */
        struct MediaDescription {
            /**
             * \brief The path to the folder containing the sprites
             */
            std::filesystem::path baseSprites = "sprites/";
            /**
             * \brief The path to the folder containing the maps
             */
            std::filesystem::path mapFolder = "maps/";
            /**
             * \brief The path to the folder containing the systems
             */
            std::filesystem::path systemsFolder = "systems/";
            /**
             * \brief The path to the folder containing the fonts
             */
            std::filesystem::path fontsFolder = "fonts/";
            /**
             * \brief The path to the folder containing the entities descriptions
             */
            std::filesystem::path entitiesFolder = "entities/";
            /**
             * \brief The path to the folder containing the levels
             */
            std::filesystem::path levelsFolder = "levels/";
            /**
             * \brief The path to the folder containing the states
             */
            std::filesystem::path statesFolder = "states/";
            /**
             * \brief The path to the JSON file describing the input bindings
             */
            std::filesystem::path inputDescription = "keys.json";
            /**
             * \brief The path to the "media" folder
             */
            std::filesystem::path baseMediaPath = "";
        };

        /**
         * \brief Describes how the states work in the engine
         */
        struct StatesDescription {
            /**
             * \brief The state to launch when the application starts
             */
            std::string firstState = "intro";
        };

    public:
        /**
         * \brief Registers Lua functions for the game description
         * \param lua The Lua state
         */
         void luaFunctions(sol::state &lua);


    public:
        /**
         * \brief The log description
         */
        LogDescription log;
        /**
         * \brief The window description
         */
        WindowDescription window;
        /**
         * \brief The media description
         */
        MediaDescription media;
        /**
         * \brief The states description
         */
        StatesDescription states;
        /**
         * \brief The version of the game
         * 
         * Recommanded format: x.x.x
         */
        std::string version = "";
        /**
         * \brief The name of the game
         */
        std::string name = "";
    };
    
    /**
     * \brief Constructs the log description from a JSON object
     */
    void from_json(const nlohmann::json &j, GameDescription::LogDescription &l);

    /**
     * \brief Constructs the window description from a JSON object
     */
    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w);

    /**
     * \brief Constructs the media description from a JSON object
     */
    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m);

    /**
     * \brief Constructs the staets description from a JSON object
     */
    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s);

    /**
     * \brief Constructs the game description from a JSON object
     */
    void from_json(const nlohmann::json &j, GameDescription &g);
}