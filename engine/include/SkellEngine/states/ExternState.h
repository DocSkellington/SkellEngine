#pragma once

#include "SkellEngine/states/State.h"

namespace engine::states {
    /**
     * \brief A state defined in a Lua script
     */
    class ExternState : public State {
    public:
        /**
         * \brief Constructor
         * \param manager The state manager
         * \param stateName The name of the state
         */
        ExternState(StateManager &manager, const std::string &stateName);
        ~ExternState();

        virtual void onCreate() override;
        virtual void onDestroy() override;
        virtual void activate() override;
        virtual void deactivate() override;
        virtual void update(sf::Int64 deltatime) override;
        virtual void draw(sf::RenderTarget &target) override;

    private:
        void luaFunctions();

    private:
        sol::state m_lua;
        std::string m_stateName;
    };
}