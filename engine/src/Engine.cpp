#include "Engine.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>
#include <cstdio>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <json.hpp>

#include "config.h"
#include "states/MainMenuState.h"
#include "files/FileManager.h"
#include <tmxlite/Log.hpp>

using namespace std;

namespace engine {
    Engine::Engine() {
        #ifdef DEBUG
        // Erasing the old log
        remove("media/log.txt");
        // Setting default value to the output
        tmx::Logger::setOutput(tmx::Logger::Output::All);
        #else
        tmx::Logger::setOutput(tmx::Logger::Output::None);
        #endif

        m_context.fileManager = make_shared<files::FileManager>(m_context);

        m_context.lua = make_shared<sol::state>();
        m_context.lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        m_context.textureHolder = make_shared<thor::ResourceHolder<sf::Texture, std::string>>();

        m_context.stateManager = make_shared<states::StateManager>(m_context);
        
        m_context.entityManager = make_shared<entities::EntityManager>(m_context);

        m_context.systemManager = make_shared<systems::SystemManager>(m_context);

        // Opening the config file
        auto description = m_context.fileManager->getGameDescription();

        #ifdef DEBUG
        tmx::Logger::setOutput(description.log.output);
        #endif

        m_context.map = make_shared<map::Map>(m_context, description.media.mapFolder);

        // Creating the window
        createWindow(description.window, description.version);

        // Launching the first state of the game
        if (description.states.firstState == "mainmenu" || description.states.firstState == "mainmenustate")
            m_context.stateManager->switchTo<states::MainMenuState>();
    }

    Engine::~Engine() {

    }

    void Engine::run() {
        sf::Clock clock;
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

            sf::Time elapsed = clock.restart();

            m_context.stateManager->update(elapsed.asMicroseconds());

            m_context.stateManager->draw(m_context.window);

            m_context.stateManager->processRemove();
        }
    }

    void Engine::createWindow(const files::GameDescription::WindowDescription &window, const std::string &version) {
        string title = window.title;
        if (window.version)
            title += " - v." + version;
        if (window.engine)
            title += " - ENGINE v." + to_string(ENGINE_VERSION_MAJOR) + "." + to_string(ENGINE_VERSION_MINOR) + "." + to_string(ENGINE_VERSION_REVISION);

        int style = sf::Style::Titlebar;
        if (window.resize)
            style |= sf::Style::Resize;
        if (window.close)
            style |= sf::Style::Close;
        if (!window.titlebar)
            style = sf::Style::None;

        if (window.fullscreen)
            m_context.window = make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        else
            m_context.window = make_shared<sf::RenderWindow>(sf::VideoMode(window.width, window.height, 32), title, style);

    }
}