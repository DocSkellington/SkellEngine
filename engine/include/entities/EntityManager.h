#pragma once

#include <vector>

#include "Entity.h"

namespace engine::entities {
    struct Context;

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
         */
        void addEntity(const std::string &type);
        /**
         * \brief Adds an entity of the given type and constructs it with the given JSON table
         * \param type The type of the entity
         * \param entityDescription The JSON table describing the entity
         */
        void addEntity(const std::string &type, const nlohmann::json &entityDescription);
        /**
         * \brief Adds an entity of the given type and constructs it with the given Lua table/script
         * \param type The type of the entity
         * \param entityScript The Lua table/script describing the entity
         */
        void addEntity(const std::string &type, const sol::table &entityScript);
        /**
         * \brief Adds an entity of the given type and constructs it with the given JSON table and Lua table/script
         * \param type The type of the entity
         * \param entityDescription The JSON table describing the entity
         * \param entityScript The Lua table/script describing the entity
         */
        void addEntity(const std::string &type, const nlohmann::json &entityDescription, const sol::table &entityScript);

    private:
        std::vector<Entity::Ptr> m_entities;
        Context &m_context;
    };
}