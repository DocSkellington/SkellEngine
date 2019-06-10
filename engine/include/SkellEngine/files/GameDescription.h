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
     * \ingroup Engine
     * 
     * It should be loaded trough JSON. It contains every information global to the entire game (such as the window's parameters, the media and so on).
     */
    struct GameDescription {
    public:
        struct LogDescription {
            tmx::Logger::Output output;
            std::filesystem::path logPath;
        };

        /**
         * \brief Describes the window.
         * 
         * It should be loaded trough JSON.
         */
        struct WindowDescription {
            bool fullscreen, titlebar, resize, close, version, engine;
            std::string title;
            int width, height, antialiasingLevel;
        };

        /**
         * \brief Describes the media folder.
         * 
         * It should be loaded trough JSON.
         */
        struct MediaDescription {
            std::filesystem::path baseSprites;
            std::filesystem::path mapFolder;
            std::filesystem::path systemsFolder;
            std::filesystem::path fontsFolder;
            std::filesystem::path entitiesFolder;
            std::filesystem::path levelsFolder;
            std::filesystem::path statesFolder;
        };

        /**
         * \brief Describes the first state of the game.
         */
        struct StatesDescription {
            std::string firstState;
        };

    public:
        LogDescription log;
        WindowDescription window;
        MediaDescription media;
        StatesDescription states;
        std::string version, name;
    };
    
    void from_json(const nlohmann::json &j, GameDescription::LogDescription &l);

    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w);

    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m);

    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s);

    void from_json(const nlohmann::json &j, GameDescription &g);
}