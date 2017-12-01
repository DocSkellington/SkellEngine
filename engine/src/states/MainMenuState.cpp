#include "states/MainMenuState.h"

#include <iostream>

#include "states/GameState.h"

namespace engine::states {
    MainMenuState::MainMenuState(StateManager& manager) :
        State(manager) {

    }

    MainMenuState::~MainMenuState() {

    }

    void MainMenuState::onCreate() {
        std::cout << "CREATING\n";
    }

    void MainMenuState::onDestroy() {

    }

    void MainMenuState::activate() {
        std::cout << "MAIN MENU\n";
    }

    void MainMenuState::deactivate() {

    }

    void MainMenuState::update(float deltatime) {

    }

    void MainMenuState::handleEvent(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            getStateManager().switchTo<GameState>();
            getStateManager().remove<MainMenuState>();
        }
    }

    void MainMenuState::draw(std::shared_ptr<sf::RenderWindow>& window) {
    }
}