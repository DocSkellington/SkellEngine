#pragma once

#include "SkellEngine/states/State.h"

namespace engine::states {
    /**
    * \brief Default implementation of the Game state
    * \ingroup Engine
    */
    class GameState : public State {
    public:
        GameState(StateManager& manager);
        GameState(const GameState&) = delete;
        virtual ~GameState();

        virtual void onCreate() override;
        virtual void onDestroy() override;

        virtual void activate() override;
        virtual void deactivate() override;

        virtual void update(sf::Int64 deltatime) override;
        virtual void handleEvent(sf::Event &event) override;
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window) override;
    };
}