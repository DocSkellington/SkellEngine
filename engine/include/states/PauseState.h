#pragma once

#include "states/State.h"

namespace engine::states {
    class PauseState : public State {
        PauseState(StateManager& manager);
        PauseState(const PauseState&) = delete;
        virtual ~PauseState();

        virtual void onCreate();
        virtual void onDestroy();

        virtual void activate();
        virtual void deactivate();

        virtual void update(float deltatime);
        virtual void handleEvent(sf::Event &event);
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window);
    };
}