#pragma once

#include <sol/sol.hpp>

#include "SkellEngine/systems/System.h"

namespace engine::systems {
    /**
     * \brief A system defined in a Lua script
     */
    class ExternSystem : public System {
    public:
        /**
         * \brief Specific System::Ptr for the ExternSystem
         */
        typedef std::shared_ptr<ExternSystem> Ptr;

    public:
        /**
         * \brief The constructor
         * \param manager The system manager
         */
        explicit ExternSystem(SystemManager &manager);
        ExternSystem(const ExternSystem&) = delete;
        virtual ~ExternSystem();

        /**
         * \brief Loads the system from a Lua script
         * \param systemName The name of the system to load
         */
        virtual void loadLua(const std::string &systemName);

        virtual bool update(sf::Int64 deltatime, sf::View &view) override;

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override;

    private:
        sol::state m_lua;
        std::string m_systemName;
    };
}