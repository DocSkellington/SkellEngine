#pragma once

#include <vector>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <json.hpp>

namespace engine::files {
    struct GameDescription {
    public:
        struct WindowDescription {
            bool fullscreen, titlebar, resize, close, version, engine;
            std::string title;
            int width, height, aliasingLevel;
        };

        struct MediaDescription {
            std::string baseSprites;
            std::array<std::string, 3> maps; // 0: maps, 1: tilesets, 2: images
        };

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