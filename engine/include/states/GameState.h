#pragma once

#include "states/State.h"

namespace engine::states {
    /**
    * \brief Default implementation of the Game state
    */
    class GameState : public State {
    public:
        GameState(StateManager& manager);
        GameState(const GameState&) = delete;
        virtual ~GameState();

        virtual void onCreate();
        virtual void onDestroy();

        virtual void activate();
        virtual void deactivate();

        virtual void update(float deltatime);
        virtual void handleEvent(sf::Event &event);
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window);
    };
}