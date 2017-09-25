#include "Engine.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <json.hpp>

#include "config.h"
#include "states/MainMenuState.h"

using namespace std;

namespace engine {
    Engine::Engine() {
        // Opening the config file
        ifstream file("media/game.json");
        nlohmann::json description;
        file >> description;
        
        auto game = description["game"];
        string version = game["version"];
        string firstState = game["firstState"];
        transform(firstState.begin(), firstState.end(), firstState.begin(), ::tolower);

        context.lua = std::make_shared<sol::state>();
        context.lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        // Retrieving window's configuration (size, style and title)
        auto window = description["window"];
        auto size = window["size"];
        string title = window["title"];
        if (window["showVersion"])
            title += " - v." + version;
        if (window["showEngine"])
            title += " - ENGINE v." + to_string(ENGINE_VERSION_MAJOR) + "." + to_string(ENGINE_VERSION_MINOR) + "." + to_string(ENGINE_VERSION_REVISION);

        int style = sf::Style::None;
        if (window["showTitlebar"].get<bool>())
            style |= sf::Style::Titlebar;
        if (window["showResize"].get<bool>())
            style |= sf::Style::Resize;
        if (window["showClose"].get<bool>())
            style |= sf::Style::Close;

        if (window["fullscreen"])
            context.window = make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        else
            context.window = make_shared<sf::RenderWindow>(sf::VideoMode(size[0], size[1], 32), title, style);

        context.stateManager = make_shared<states::StateManager>();

        // Launching the first state of the game
        if (firstState == "mainmenu" || firstState == "mainmenustate")
            context.stateManager->switchTo<states::MainMenuState>();
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