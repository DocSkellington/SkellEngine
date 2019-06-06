#include "SkellEngine/states/GameState.h"

#include <iostream>

#include "SkellEngine/states/MainMenuState.h"
#include "SkellEngine/Context.h"

namespace engine::states {
    GameState::GameState(StateManager& manager) :
        State(manager) {

    }

    GameState::~GameState() {

    }

    void GameState::onCreate() {
        getStateManager().getContext().systemManager->clear();
        auto game = getStateManager().getContext().fileManager->getStateDescription("game");
        getStateManager().getContext().systemManager->loadSystems(game.systems);
        //getStateManager().getContext().window->setView(sf::View(sf::Vector2f(310, 310), sf::Vector2f(640, 640)));
    }

    void GameState::onDestroy() {
        getStateManager().getContext().gui->removeAllWidgets();
    }

    void GameState::activate() {
        // Tests:
        std::string levelName = "tutorial";
        getStateManager().getContext().fileManager->changeLevel(levelName);
    }

    void GameState::deactivate() {

    }

    void GameState::update(sf::Int64 deltatime) {
        getStateManager().getContext().systemManager->update(deltatime);
        getStateManager().getContext().map->updateLayers(deltatime);
    }

    void GameState::handleEvent(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            getStateManager().switchTo<MainMenuState>();
            getStateManager().remove<GameState>();
        }
    }

    void GameState::draw(std::shared_ptr<sf::RenderWindow>& window) {
        for (std::size_t i = 0 ; i < getStateManager().getContext().map->getLayerCount() ; i++) {
            getStateManager().getContext().systemManager->draw(window.get(), i);
        }
    }
}