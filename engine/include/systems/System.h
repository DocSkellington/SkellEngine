#pragma once

#include "entities/Entity.h"

namespace engine::systems {
    class SystemManager;

    /**
     * \brief Defines the base of every system.
     */
    class System {
    public:
        typedef std::shared_ptr<System> Ptr;

    public:
        System(SystemManager& manager);
        System(const SystemManager&) = delete;
        virtual ~System();

        /**
         * \brief Updates the system.
         * 
         * It must be define in each system
         * \param deltatime The time since the last update
         * \return Whether the update could correctly be updated or not
         */
        virtual bool update(float deltatime) = 0;

        /**
         * \brief Tries to add an entity to this system.
         * 
         * If the system does not meet the requirements, it is not added (see checkComponents).
         * \param entity The shared pointer to the entity to add
         * \return Whether the entity could be added or not
         */
        virtual bool addEntity(engine::entities::Entity::Ptr entity);

        /**
         * \brief Tries to remove an entity from this system.
         * 
         * If the entity is not in the system, nothing happens.
         * \param entity The shared pointer to the entity to remove
         */
        bool removeEntity(engine::entities::Entity::Ptr entity);
        
        /**
         * \brief Creates an instance of a system based on the given name. If the type is unknown, a generic system is returned.
         * 
         * The system must be registered to be known (see RegisterSystem).
         * \param systemName The name of the system to create
         * \param manager The SystemManager
         */
        static Ptr createInstance(const std::string &systemName, SystemManager& manager);

    protected:
        typedef std::map<std::string, std::function<Ptr(SystemManager&)>> MapType;

        /**
         * \brief Returns the (name, constructor) map
         */
        static MapType &getMapToSystem();

        /**
         * \brief Structure to use when you want to register a system.
         * 
         * Create an instance of this structure in the .h file, then construct it in the .cpp file
         * \tparam The type/name of the system you want to register
         */
        template <typename T>
        struct RegisterSystem {
            /**
             * \brief Link the name of the system to its constructor
             * \param name The name of the system
             */
            RegisterSystem (const std::string &name) {
                getMapToSystem().insert(std::make_pair(name, std::make_shared<T>));
            }
        };

    protected:
        /**
         * \brief Returns the system manager
         * \return The SystemManager
         */
        SystemManager& getSystemManager();

        /**
         * \brief Checks if the entity meets the requirements of this system (if it has the needed components).
         * \paramn entity The shared pointer to the entity to check
         * \return Whether the entity has the needed components or not
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
        static MapType m_nameToSystem;
    };
}