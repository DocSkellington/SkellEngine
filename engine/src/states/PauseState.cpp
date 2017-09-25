#include "states/PauseState.h"

#include <iostream>

#include "states/GameState.h"

namespace engine::states {
    PauseState::PauseState(StateManager& manager) :
        State(manager) {

    }

    PauseState::~PauseState() {

    }

    void PauseState::onCreate() {
    }

    void PauseState::onDestroy() {

    }

    void PauseState::activate() {
    }

    void PauseState::deactivate() {

    }

    void PauseState::update(float deltatime) {

    }

    void PauseState::handleEvent(sf::Event &event) {
    }

    void PauseState::draw(std::shared_ptr<sf::RenderWindow>& window) {
        window->display();
    }
}