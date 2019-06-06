#pragma once

#include <SFML/Graphics/View.hpp>

#include "SkellEngine/entities/Entity.h"

namespace engine::systems {
    class SystemManager;

    /**
     * \brief Defines the base of every system.
     * \ingroup Engine
     * 
     * If you define your system, you must register it before being able to use it. Please, see RegisterSystem
     */
    class System {
    public:
        /**
         * \brief The type to use for a pointer to a System.
         */
        typedef std::shared_ptr<System> Ptr;

    public:
        explicit System(SystemManager& manager);
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
         * \brief The type of the internal container mapping the name of a system to its constructor.
         */
        typedef std::map<std::string, std::function<Ptr(SystemManager&)>> MapType;

        /**
         * \brief Returns the (name, constructor) map
         */
        static std::shared_ptr<MapType> getMapToSystem();

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
                if (name == "extern") {
                    tmx::Logger::log(name + " is not a valid name for a system.", tmx::Logger::Type::Warning);
                }
                else {
                    std::function<Ptr(SystemManager&)> func = [](SystemManager &manager) {
                        return std::make_shared<T>(manager);
                    };
                    getMapToSystem()->insert(std::make_pair(name, func));
                }
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