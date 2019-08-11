#pragma once

#include "SkellEngine/states/State.h"
#include "SkellEngine/states/StateManager.h"

namespace engine::states {
    /**
    * \brief Default implementation of the main menu state
    */
    class MainMenuState : public State {
    public:
        /**
         * \brief The constructor
         * \param manager The state manager
         */
        MainMenuState(StateManager& manager);
        MainMenuState(const MainMenuState&) = delete;
        virtual ~MainMenuState();

        virtual void onCreate() override;
        virtual void onDestroy() override;

        virtual void activate() override;
        virtual void deactivate() override;

        virtual void update(sf::Int64 deltatime) override;
        virtual void handleEvent(sf::Event &event) override;
        virtual void draw(sf::RenderWindow &window) override;

        REGISTER_STATE(MainMenuState, "mainmenu")
    };
}