#pragma once

#include "SkellEngine/states/State.h"

namespace engine::states {
    class ExternState : public State {
    public:
        ExternState(StateManager &manager, const std::string &stateName);
        ~ExternState();

        virtual void onCreate() override;
        virtual void onDestroy() override;
        virtual void activate() override;
        virtual void deactivate() override;
        virtual void update(sf::Int64 deltatime) override;
        virtual void draw(sf::RenderWindow &window) override;

    private:
        sol::state m_lua;
        std::string m_stateName;
    };
}