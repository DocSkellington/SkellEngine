#pragma once

#include <json.hpp>

#include "systems/System.h"
#include "systems/GraphicalSystem.h"

namespace engine {
    struct Context;
}

/**
 * \brief Defines the basic systems
 */
namespace engine::systems {
    /**
     * \brief The system manager of the engine
     */
    class SystemManager final {
    public:
        explicit SystemManager(Context& context);
        ~SystemManager();

        /**
         * \brief Updates the system manager
         * \param deltatime The time since the last update
         */
        void update(float deltatime);
        /**
         * \brief Draws the system manager
         * \param window The window in which to draw
         * \param layer The layer to draw
         */
        void draw(sf::RenderWindow* window, unsigned int layer);

        /**
         * \brief Returns the system of the given type, if it exists in the manager
         * \param name The name of the system
         * \return A shared pointer to the system of the given type. It the system is not in the manager, the pointer is empty.
         */
        System::Ptr getSystem(const std::string &name);

        /**
         * \brief Adds a system of the given type.
         * 
         * If the system is already in the list of used systems, nothing happens.
         * \param name The name of the system to add
         * \return Whether the system could be added or not
         */
        bool addSystem(const std::string& name);

        /**
         * \brief Clears the current systems and loads every system in the given list.
         * 
         * If a system can not be loaded, the systems' list remains empty
         * \param systems The systems to create
         */
        bool loadSystems(const std::vector<std::string> &systems);

        /**
         * \brief Removes the system of the given name, if it is used.
         * 
         * If the system is not in the manager, nothing happens.
         * \param name The name of the system to remove
         */
        bool removeSystem(const std::string &name);

        /**
         * \brief Removes every system in the manager
         */
        void clear();

        /**
         * \brief Tries to add the entity to every system.
         * 
         * If every system rejects the entity, nothing is modified.
         * \param entity The entity to add
         * \return Whether at least one system accepted the entity, or not
         */
        bool addEntity(entities::Entity::Ptr entity);
        /**
         * \brief Tries to remove the entity from every system.
         * 
         * If the entity is not used by any system, nothing is modified.
         * \param entity The entity to remove
         * \return Whether at least one system removed the entity, or not
         */
        bool removeEntity(entities::Entity::Ptr entity);

        /**
         * \brief Returns the context
         * \return The context
         */
        Context& getContext() const;

    private:
        Context &m_context;
        std::map<std::string, System::Ptr> m_systems;
        std::map<std::string, GraphicalSystem::Ptr> m_graphicalSystems;
    };
}