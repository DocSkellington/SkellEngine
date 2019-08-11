#include "SkellEngine/Engine.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>
#include <cstdio>
#include <filesystem>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/events/EventHandler.h"
#include "SkellEngine/input/InputHandler.h"
#include "SkellEngine/states/StateManager.h"

namespace engine {
    Engine::Engine(const std::filesystem::path &baseMediapath) :
        m_context(baseMediapath) {
        // Erasing the old log
        std::filesystem::path logPath = baseMediapath / "log.txt";
        std::filesystem::remove(logPath);
        // Setting default value to the output
        tmx::Logger::setOutput(tmx::Logger::Output::Console);

        m_context.eventHandler->registerCallback("WindowClosed", [&](const engine::events::Event &event) { tmx::Logger::log("Closing the window"); m_context.window->close(); });

        tmx::Logger::log("Launching the first state");
        // Launching the first state of the game
        if (m_context.fileManager->getGameDescription().states.firstState == "mainmenu" || m_context.fileManager->getGameDescription().states.firstState == "mainmenustate")
            m_context.stateManager->switchTo("mainmenu");
    }

    Engine::~Engine() {

    }

    void Engine::run() {
        sf::Clock clock;
        while (m_context.window->isOpen()) {
            m_context.inputHandler->clearInputs();

            sf::Event event;
            while (m_context.window->pollEvent(event)) {
                m_context.gui->handleEvent(event);

                m_context.inputHandler->proccess(event);
            }

            m_context.inputHandler->triggerEvents();

            sf::Time elapsed = clock.restart();

            m_context.stateManager->update(elapsed.asMicroseconds());

            m_context.window->clear();

            m_context.stateManager->draw(*m_context.window);
            m_context.gui->draw();

            m_context.window->display();

            m_context.stateManager->processRemove();
        }
    }
}