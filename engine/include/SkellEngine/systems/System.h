/**
 * \file System.h
 */

#pragma once

#include <SFML/Graphics/View.hpp>

#include "SkellEngine/entities/Entity.h"
#include "SkellEngine/utilities/RegisterClass.h"
#include "SkellEngine/events/StoreEventConnections.h"

namespace engine::systems {
    class SystemManager;

    /**
     * \brief Defines the base of every system.
     * 
     * If you define your system, you must register it before being able to use it.
    * \see REGISTER_SYSTEM for an helper macro to register a system
     */
    class System : public events::StoreEventConnections {
    public:
        /**
         * \brief The type to use for a pointer to a System.
         */
        typedef std::shared_ptr<System> Ptr;

    public:
        System(const System&) = delete;
        virtual ~System();

        /**
         * \brief Updates the system.
         * 
         * It must be define in each system
         * \param deltatime The time since the last update
         * \param view The view of the game; beware that every system can modify the view
         * \return Whether the update could correctly be updated or not
         */
        virtual bool update(sf::Int64 deltatime, sf::View &view) = 0;

        /**
         * \brief Tries to add an entity to this system.
         * 
         * If the system does not meet the requirements, it is not added (see checkComponents).
         * \param entity The shared pointer to the entity to add
         * \return True if the entity could be added to the System, false otherwise
         */
        virtual bool addEntity(engine::entities::Entity::Ptr entity);

        /**
         * \brief Tries to remove an entity from this system.
         * 
         * If the entity is not in the system, nothing happens (but the function returns false).
         * \param entity The shared pointer to the entity to remove
         * \return True if the entity could be removed, false otherwise.
         */
        virtual bool removeEntity(engine::entities::Entity::Ptr entity);
        
        /**
         * \brief Creates an instance of a system based on the given name. If the type is unknown, a generic system is returned.
         * 
         * The system must be registered to be known (see RegisterSystem).
         * \param systemName The name of the system to create
         * \param manager The SystemManager
         */
        static Ptr createInstance(const std::string &systemName, SystemManager& manager);

    protected:
        /**
         * \brief The specialisation of utilities::RegisterClass for the systems
         */
        using RegisteredSystems = utilities::RegisterClass<System, SystemManager&>;

        /**
         * \brief A shortcut to register a system
         * \tparam T The type of the system to register
         */
        template <typename T>
        using RegisterSystem = RegisteredSystems::Register<T>;

    protected:
        /**
         * \brief The constructor
         * \param manager The system manager
         */
        System(SystemManager& manager);

        /**
         * \brief Returns the system manager
         * \return The SystemManager
         */
        SystemManager& getSystemManager();

        /**
         * \brief Checks if the entity meets the requirements of this system (if it has the needed components).
         * 
         * Each system must override this function to specify the components required to add an entity.
         * \param entity The shared pointer to the entity to check
         * \return True if the entity has the required components, false otherwise
         */
        virtual bool checkComponents(engine::entities::Entity::Ptr entity) const = 0;
        
        /**
         * \brief Returns the entities of the system.
         * \return The entities this system is using.
         */
        std::vector<engine::entities::Entity::Ptr>& getEntities();

    private:
        SystemManager &m_manager;
        std::vector<engine::entities::Entity::Ptr> m_entities;
    };
}

/**
 * \brief Registers the system TYPE under the name NAME
 * 
 * \warning It must be placed <b>inside</b> of the class definition. For example, do something like:
 * \code
 * class ExampleSystem : public System {
 *  public:
 *      ExampleSystem(Context& context) : System(context) { ... }
 *      ...
 *      REGISTER_SYSTEM(ExampleSystem, "example")
 * };
 * \endcode
 * 
 * \note This macro adds a private member variable. The name of the variable is the concatenation of "registeringVariable" and the line number in the header file using this macro. This allows to register multiple times the same system under different names
 * \warning This macro uses "private: ". Therefore, everything declared after this macro will be marked as private in your class definition.
 */
#define REGISTER_SYSTEM(TYPE, NAME) REGISTER_CLASS(RegisterSystem, TYPE, NAME)
