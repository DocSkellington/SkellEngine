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
        explicit Entity(Context &context);
        explicit Entity(Context &context, const std::string &type);
        Entity(const Entity&) = delete;
        ~Entity();

        /**
         * \brief Create and add an empty component into this entity.
         * \param componentType The type of the component to create
         * \return A shared pointer to the created component
         */
        components::Component::Ptr addComponent(const std::string &componentType);

        /**
         * \brief Adds a component into this entity
         * \param componentType The type of the component to create
         * \param jsonTable The JSON table to use to initialise the component
         * \return A shared pointer to the created component
         */
        components::Component::Ptr addComponent(const std::string &componentType, const nlohmann::json &jsonTable);

        /**
         * \brief Does this entity have the component?
         * \param componentType The type of the component to look for
         * \return True if the entity has the component, false otherwise
         */
        bool hasComponent(const std::string &componentType) const;

        /**
         * \brief Gets the component, if it exists
         * \param componentType The type of the component to look for
         * \return A shared pointer to the component (empty if the entity does not have the component)
         */
        components::Component::Ptr getComponent(const std::string &componentType);

        /**
         * \brief Returns the number of components this entity has
         * \return The number of components this entity has
         */
        std::size_t getNumberOfComponents();

        /**
         * \brief Returns a vector with the names of the components this entity has.
         * 
         * The vector is sorted.
         * \return A vector of strings with the names of components this entity has
         */
        std::vector<std::string> getComponentsNames();

        /**
         * \brief Returns the type of the entity
         * \return The type of the entity
         */
        const std::string& getType() const;

        /**
         * \brief Registers the Lua functions associated with this class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    private:
        Context &m_context;
        std::string m_type;
        std::map<std::string, components::Component::Ptr> m_components;

    private:
        /**
         * \brief Adds a component into this entity
         * \param componentType The type of the component to create
         * \param luaTable The Lua table
         * \return A shared pointer to the created component
         */
        components::Component::Ptr addComponent(const std::string &componentType, const sol::table &luaTable);
    };
}