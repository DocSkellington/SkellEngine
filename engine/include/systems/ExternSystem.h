#pragma once

#include <sol.hpp>

#include "systems/System.h"

namespace engine::systems {
    /**
     * \brief A system defined in a Lua script
     * \ingroup Engine
     */
    class ExternSystem : public System {
    public:
        typedef std::shared_ptr<ExternSystem> Ptr;

    public:
        explicit ExternSystem(SystemManager &manager);
        ExternSystem(const ExternSystem&) = delete;
        virtual ~ExternSystem();

        virtual void loadLua(const std::string &systemName);

        virtual bool update(sf::Int64 deltatime, sf::View &view) override;

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const;

    private:
        sol::state m_lua;
    };
}