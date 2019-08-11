#include "SkellEngine/states/GameState.h"

#include <iostream>

#include "SkellEngine/Context.h"
#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/map/Map.h"
#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/levels/Level.h"

namespace engine::states {
    GameState::GameState(StateManager& manager) :
        State(manager) {

    }

    GameState::~GameState() {

    }

    void GameState::onCreate() {
        getStateContext().systemManager->clear();

        auto game = getStateContext().context.fileManager->getStateDescription("game");

        getStateContext().systemManager->loadSystems(game.systems);
        //getStateContext().window->setView(sf::View(sf::Vector2f(310, 310), sf::Vector2f(640, 640)));
    }

    void GameState::onDestroy() {
        getStateContext().systemManager->clear();
        getStateContext().context.gui->removeAllWidgets();
    }

    void GameState::activate() {
        std::string levelName = "tutorial";
        getStateContext().level->changeLevel(levelName);
    }

    void GameState::deactivate() {

    }

    void GameState::update(sf::Int64 deltatime) {
        getStateContext().systemManager->update(deltatime);
        getStateContext().level->update(deltatime);
    }

    void GameState::handleEvent(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            getStateContext().context.stateManager->switchTo("mainmenu");
            getStateContext().context.stateManager->remove("game");
        }
    }

    void GameState::draw(sf::RenderWindow& window) {
        getStateContext().level->draw(window);
    }
}