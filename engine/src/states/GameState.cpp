#include "states/GameState.h"

#include <iostream>

#include "states/MainMenuState.h"
#include "Context.h"

namespace engine::states {
    GameState::GameState(StateManager& manager) :
        State(manager) {

    }

    GameState::~GameState() {

    }

    void GameState::onCreate() {
        getStateManager().getContext().systemManager->clear();
        auto game = getStateManager().getContext().fileManager->getGameDescription();
        //auto &systems = game.at("systems");
        //getStateManager().getContext().systemManager->loadSystems(systems);
    }

    void GameState::onDestroy() {

    }

    void GameState::activate() {
        // Tests:
        std::string levelName = "tutorial";
        getStateManager().getContext().fileManager->changeLevel(levelName);
    }

    void GameState::deactivate() {

    }

    void GameState::update(float deltatime) {
        getStateManager().getContext().systemManager->update(deltatime);
    }

    void GameState::handleEvent(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            getStateManager().switchTo<MainMenuState>();
            getStateManager().remove<GameState>();
        }
    }

    void GameState::draw(std::shared_ptr<sf::RenderWindow>& window) {
        getStateManager().getContext().systemManager->draw(window.get(), 0);
    }
}