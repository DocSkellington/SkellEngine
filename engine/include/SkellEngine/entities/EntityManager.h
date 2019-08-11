#pragma once

#include <vector>

#include "SkellEngine/entities/Entity.h"
#include "SkellEngine/states/StateContext.h"

namespace engine::entities {
    /**
     * \brief Handles the entities of the engine
     */
    class EntityManager final {
    public:
        /**
         * \brief The constructor
         * \param context The context
         */
        explicit EntityManager(states::StateContext &context);
        EntityManager(const EntityManager&) = delete;

        /**
         * \brief Removes the entity from the engine.
         * 
         * It also removes the entity from the SystemManager (and the systems).
         * \param entity The entity to remove
         * \brief True if the entity could be removed, false otherwise
         */
        bool removeEntity(Entity::Ptr entity);

        /**
         * \brief Adds an empty entity of the given type
         * 
         * If the entity is constructed this way, it must be manually added in the system manager to be used by systems
         * \param type The type of the entity
         * \return A shared pointer to the entity
         * \see engine::systems::SystemManager::addEntity to add the entity
         */
        Entity::Ptr addEntity(const std::string &type);
        /**
         * \brief Adds an entity of the given type and constructs it with the given JSON table
         * 
         * It also adds the entity in the system manager (and the systems)
         * \param type The type of the entity
         * \param entityDescription The JSON table describing the entity
         * \return A shared pointer to the entity
         */
        Entity::Ptr addEntity(const std::string &type, const nlohmann::json &entityDescription);

        /**
         * \brief Gets the first entity of the given type
         * \param type The type of the entity
         * \return A shared pointer to the first entity that has the given type
         */
        Entity::Ptr getEntity(const std::string &type);

        /**
         * \brief Gets the first entity that has the given type and that has at least the given components.
         * \param type The type of the entity
         * \param components The components the entity must have. The entity can have another components
         * \return A shared pointer to the first entity that has the given type
         */
        Entity::Ptr getEntity(const std::string &type, std::vector<std::string> components);

        /**
         * \brief Gets the number of entities
         * \return The number of entities in the engine
         */
        std::size_t getNumberOfEntities() const;

        /** @{ */
        /**
         * \brief Gives the context of the state using this manager
         * \return The context of the state using this manager
         */
        states::StateContext& getContext();
        const states::StateContext &getContext() const;
        /** @} */

        /**
         * \brief Registers the Lua functions associated with this class
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    private:
        std::vector<Entity::Ptr> m_entities;
        states::StateContext &m_context;

    private:
        Entity::Ptr addEntity(const std::string &name, const sol::table &luaTable);
        Entity::Ptr getEntity(const std::string &name, const sol::table &components);
    };
}