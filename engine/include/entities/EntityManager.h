#pragma once

#include <vector>

#include "Entity.h"

namespace engine {
    struct Context;
}

namespace engine::entities {
    /**
     * \brief Handles the entities of the engine
     */
    class EntityManager final {
    public:
        explicit EntityManager(Context &context);
        EntityManager(const EntityManager&) = delete;

        /**
         * \brief Adds an empty entity of the given type
         * \param type The type of the entity
         * \return A shared pointer to the entity
         */
        Entity::Ptr addEntity(const std::string &type);
        /**
         * \brief Adds an entity of the given type and constructs it with the given JSON table
         * \param type The type of the entity
         * \param entityDescription The JSON table describing the entity
         * \return A shared pointer to the entity
         */
        Entity::Ptr addEntity(const std::string &type, const nlohmann::json &entityDescription);

        /**
         * \brief Registers the Lua functions associated with this class
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    private:
        std::vector<Entity::Ptr> m_entities;
        Context &m_context;
    };
}