#pragma once

#include <typeindex>
#include <memory>

#include "entities/components/Component.h"

/**
 * \brief Contains everything related to the entities of a game
 */
namespace engine::entities {
    /**
     * \brief An entity is made of zero, one or multiple components.
     * 
     * An entity with zero components is called "empty".
     * An entity has a 'type', a name
     */
    class Entity final {
    public:
        typedef std::shared_ptr<Entity> Ptr;
    
    public:
        explicit Entity();
        explicit Entity(const std::string &type);
        Entity(const Entity&) = delete;
        ~Entity();

        /**
         * \brief Adds a component into this entity
         * \param componentType The type of the component to create
         * \param jsonTable The JSON table to use to initialise the component
         */
        void addComponent(const std::string &componentType, const nlohmann::json &jsonTable);
        /**
         * \brief Adds a component into this entity
         * \param componentType The type of the component to create
         * \param luaTable The Lua table/script to use to initialise the component
         */
        void addComponent(const std::string &componentType, const sol::table &luaTable);
        /**
         * \brief Adds a component into this entity
         * \param componentType The type of the component to create
         * \param jsonTable The JSON table to use to initialise the component
         * \param luaTable The Lua table/script to use to initialise the component
         */
        void addComponent(const std::string &componentType, const nlohmann::json &jsonTable, const sol::table &luaTable);

        bool hasComponent(const std::string &componentType) const;

        components::Component::Ptr getComponent(const std::string &componentType);

    private:
        std::string m_type;
        /** \todo Find a better way to store to allow multiple instances of the same component (multimap?) */
        std::map<std::string, components::Component::Ptr> m_components;
    };
}