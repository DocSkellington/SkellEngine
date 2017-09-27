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
#include "FileManager.h"

using namespace std;

namespace engine {
    Engine::Engine() {
        m_context.fileManager = make_shared<FileManager>(m_context);

        m_context.lua = make_shared<sol::state>();
        m_context.lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        m_context.stateManager = make_shared<states::StateManager>();

        // Opening the config file
        auto description = m_context.fileManager->getGameDescription();
        
        auto game = description["game"];
        string version = game["version"];
        string firstState = game["firstState"];
        transform(firstState.begin(), firstState.end(), firstState.begin(), ::tolower);

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
            m_context.window = make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        else
            m_context.window = make_shared<sf::RenderWindow>(sf::VideoMode(size[0], size[1], 32), title, style);

        // Launching the first state of the game
        if (firstState == "mainmenu" || firstState == "mainmenustate")
            m_context.stateManager->switchTo<states::MainMenuState>();

        // Tests:
        m_context.levelName = "tutorial";
        auto test = m_context.fileManager->getEntityJSON("test");
        std::cout << test.dump(4) << '\n';
    }

    Engine::~Engine() {

    }

    void Engine::run() {
        while (m_context.window->isOpen()) {
            sf::Event event;
            while (m_context.window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    m_context.window->close();
                }
                else {
                    m_context.stateManager->handleEvent(event);
                }
            }

            m_context.stateManager->draw(m_context.window);

            m_context.stateManager->processRemove();
        }
    }
}