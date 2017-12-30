#pragma once

#include <vector>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <json.hpp>

namespace engine::files {
    /**
     * \brief The game's parameters.
     * 
     * It should be loaded trough JSON. It contains every information global to the entire game (such as the window's parameters, the media and so on).
     */
    struct GameDescription {
    public:
        /**
         * \brief Describes the window.
         * 
         * It should be loaded trough JSON.
         */
        struct WindowDescription {
            bool fullscreen, titlebar, resize, close, version, engine;
            std::string title;
            int width, height, aliasingLevel;
        };

        /**
         * \brief Describes the media folder.
         * 
         * It should be loaded trough JSON.
         */
        struct MediaDescription {
            std::string baseSprites;
            std::array<std::string, 3> maps; // 0: maps, 1: tilesets, 2: images
        };

        /**
         * \brief Describes the first state of the game.
         */
        struct StatesDescription {
            std::string firstState;
        };

    public:
        WindowDescription window;
        MediaDescription media;
        StatesDescription states;
        std::string version, name;
    };
    
    void from_json(const nlohmann::json &j, GameDescription::WindowDescription &w);

    void from_json(const nlohmann::json &j, GameDescription::MediaDescription &m);

    void from_json(const nlohmann::json &j, GameDescription::StatesDescription &s);

    void from_json(const nlohmann::json &j, GameDescription &g);
}