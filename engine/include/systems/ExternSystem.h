#pragma once

#include <sol.hpp>

#include "systems/System.h"

namespace engine::systems {
    class ExternSystem : public System {
    public:
        typedef std::shared_ptr<ExternSystem> Ptr;

    public:
        explicit ExternSystem(SystemManager &manager);
        ExternSystem(const ExternSystem&) = delete;
        virtual ~ExternSystem();

        virtual void setLuaScript(sol::table &lua);

        virtual bool update(sf::Int64 deltatime, sf::View &view);

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const;

    private:
        sol::table m_lua;
    };
}