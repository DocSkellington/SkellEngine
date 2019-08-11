#include "SkellEngine/states/PauseState.h"

#include <iostream>

#include "SkellEngine/states/GameState.h"

namespace engine::states {
    PauseState::PauseState(StateManager& manager) :
        State(manager) {

    }

    PauseState::~PauseState() = default;

    void PauseState::onCreate() {
    }

    void PauseState::onDestroy() {

    }

    void PauseState::activate() {
    }

    void PauseState::deactivate() {

    }

    void PauseState::update(sf::Int64 deltatime) {

    }

    void PauseState::handleEvent(sf::Event &event) {
    }

    void PauseState::draw(sf::RenderWindow& window) {
    }
}