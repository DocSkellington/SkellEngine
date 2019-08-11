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
         * \brief Constructor
         * 
         * It loads the system from a Lua script
         * \param manager The system manager
         * \param systemName The name of the system to load
         */
        explicit ExternSystem(SystemManager &manager, const std::string &systemName);
        ExternSystem(const ExternSystem&) = delete;
        virtual ~ExternSystem();

        virtual bool update(sf::Int64 deltatime, sf::View &view) override;

    protected:
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override;

        /**
         * \brief Loads the system from a Lua script
         * \param systemName The name of the system to load
         */
        virtual void loadLua(const std::string &systemName);

    private:
        sol::state m_lua;
        std::string m_systemName;
    };
}