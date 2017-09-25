#include "Engine.h"

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "config.h"

using namespace std;

namespace engine {
    Engine::Engine() {
        context.lua = std::make_shared<sol::state>();
        context.lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        context.window = make_shared<sf::RenderWindow>(sf::VideoMode(800, 600, 32), "Engine v" + std::to_string(ENGINE_VERSION_MAJOR) + "." + std::to_string(ENGINE_VERSION_MINOR) + "." + std::to_string(ENGINE_VERSION_REVISION));

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