#pragma once

#include "states/State.h"

namespace engine::states {
    /**
     * \brief Default implementation of the pause state.
     * \ingroup Engine
     */ 
    class PauseState : public State {
        PauseState(StateManager& manager);
        PauseState(const PauseState&) = delete;
        virtual ~PauseState();

        virtual void onCreate() override;
        virtual void onDestroy() override;

        virtual void activate() override;
        virtual void deactivate() override;

        virtual void update(sf::Int64 deltatime) override;
        virtual void handleEvent(sf::Event &event) override;
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window) override;
    };
}