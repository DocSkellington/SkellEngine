#include "SkellEngine/states/MainMenuState.h"

#include <iostream>

#include "SkellEngine/states/GameState.h"
#include <SkellEngine/tmxlite/Log.hpp>

namespace engine::states {
    MainMenuState::MainMenuState(StateManager& manager) :
        State(manager) {

    }

    MainMenuState::~MainMenuState() {

    }

    void MainMenuState::onCreate() {
        tmx::Logger::log("CREATING", tmx::Logger::Type::Info);
    }

    void MainMenuState::onDestroy() {

    }

    void MainMenuState::activate() {
        tmx::Logger::log("MAIN MENU", tmx::Logger::Type::Info);
    }

    void MainMenuState::deactivate() {

    }

    void MainMenuState::update(sf::Int64 deltatime) {

    }

    void MainMenuState::handleEvent(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            getStateManager().switchTo("game");
            getStateManager().remove("mainmenu");
        }
    }

    void MainMenuState::draw(std::shared_ptr<sf::RenderWindow>& window) {
    }
}