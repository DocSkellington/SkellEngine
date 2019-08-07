#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include <SkellEngine/tmxlite/Log.hpp>

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
             * \brief Where the logging messages are printed (in the standard output, in a file or in both)
             */
            tmx::Logger::Output output;
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
            bool fullscreen;
            /**
             * \brief Whether to show the titlebar (in windowed mode)
             */
            bool titlebar;
            /**
             * \brief Whether to allow the user to resize the window (in windowed mode)
             */
            bool resize;
            /**
             * \brief Whether to show the close button (in windowed mode)
             */
            bool close;
            /**
             * \brief Whether to show the version of the game in the titlebar (in windowed mode and if the titlebar is shown)
             */
            bool version;
            /**
             * \brief Whether to show the name of the engine in the titlebar (in windowed mode and if the titlebar is shown)
             */
            bool engine;
            /**
             * \brief The title to set in the titlebar (in windowed mode and if the titlebar is shown)
             */
            std::string title;
            /**
             * \brief The width of the window (in windowed mode)
             */
            int width;
            /**
             * \brief The height of the window (in windowed mode)
             */
            int height;
            /**
             * \brief The level of anti-aliasing
             */
            int antialiasingLevel;
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
            std::filesystem::path baseSprites;
            /**
             * \brief The path to the folder containing the maps
             */
            std::filesystem::path mapFolder;
            /**
             * \brief The path to the folder containing the systems
             */
            std::filesystem::path systemsFolder;
            /**
             * \brief The path to the folder containing the fonts
             */
            std::filesystem::path fontsFolder;
            /**
             * \brief The path to the folder containing the entities descriptions
             */
            std::filesystem::path entitiesFolder;
            /**
             * \brief The path to the folder containing the levels
             */
            std::filesystem::path levelsFolder;
            /**
             * \brief The path to the folder containing the states
             */
            std::filesystem::path statesFolder;
            /**
             * \brief The path to the JSON file describing the input bindings
             */
            std::filesystem::path inputDescription;
        };

        /**
         * \brief Describes how the states work in the engine
         */
        struct StatesDescription {
            /**
             * \brief The state to launch when the application starts
             */
            std::string firstState;
        };

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
        std::string version;
        /**
         * \brief The name of the game
         */
        std::string name;
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