#include "Engine.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <json.hpp>

#include "config.h"

using namespace std;

namespace engine {
    Engine::Engine() {
        // Opening the config file
        std::ifstream file("media/game.json");
        nlohmann::json description;
        file >> description;
        
        auto game = description["game"];
        std::string version = game["version"];

        context.lua = std::make_shared<sol::state>();
        context.lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        // Retrieving window's configuration (size, style and title)
        auto window = description["window"];
        auto size = window["size"];
        std::string title = window["title"];
        if (window["showVersion"])
            title += " - v." + version;
        if (window["showEngine"])
            title += " - ENGINE v." + std::to_string(ENGINE_VERSION_MAJOR) + "." + std::to_string(ENGINE_VERSION_MINOR) + "." + std::to_string(ENGINE_VERSION_REVISION);

        int style = window["showTitlebar"].get<bool>() | window["showResize"].get<bool>() | window["showClose"].get<bool>();

        if (window["fullscreen"])
            context.window = make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        else
            context.window = make_shared<sf::RenderWindow>(sf::VideoMode(size[0], size[1], 32), title, style);

        context.stateManager = make_shared<states::StateManager>();
    }

    Engine::~Engine() {

    }

    void Engine::run() {
        while (context.window->isOpen()) {
            sf::Event event;
            while (context.window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    context.window->close();
                }
                else {
                    context.stateManager->handleEvent(event);
                }
            }

            context.stateManager->draw(context.window);

            context.stateManager->processRemove();
        }
    }
}