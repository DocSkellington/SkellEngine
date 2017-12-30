#include "states/MainMenuState.h"

#include <iostream>

#include "states/GameState.h"
#include "log/Logger.h"

namespace engine::states {
    MainMenuState::MainMenuState(StateManager& manager) :
        State(manager) {

    }

    MainMenuState::~MainMenuState() {

    }

    void MainMenuState::onCreate() {
        log::log("CREATING", log::LogLevel::Info);
    }

    void MainMenuState::onDestroy() {

    }

    void MainMenuState::activate() {
        log::log("MAIN MENU", log::LogLevel::Info);
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