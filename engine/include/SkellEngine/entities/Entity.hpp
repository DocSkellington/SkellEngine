#pragma once

#include <typeindex>
#include <memory>

#include "SkellEngine/entities/components/Component.hpp"

/**
 * \brief Contains everything related to the entities of a game
 */
namespace engine::entities {
    class EntityManager;

    /**
     * \brief An entity is made of zero, one or multiple components.
     * 
     * An entity with zero components is called "empty".
     * An entity has a 'type', a name
     */
    class Entity final {
    public:
        /**
         * \brief Every entity should be manipulated through this pointer type
         */
        typedef std::shared_ptr<Entity> Ptr;
        /**
         * \brief The type of the internal map
         */
        using mapType = std::map<std::string, components::Component::Ptr>;
        /**
         * \brief Allows to iterate over the components
         */
        using iterator = mapType::iterator;
        /**
         * \brief Allows to iterate (with a const iterator) over the components
         */
        using const_iterator = mapType::const_iterator;

        /**
         * \brief The type of the keys
         * 
         * Needed for Sol2 interaction
         * \see mapType
         */
        using key_type = mapType::key_type;
        /**
         * \brief The type of the values stored in the map
         * 
         * Needed for Sol2 interaction
         * \see mapType
         */
        using mapped_type = mapType::mapped_type;

        /**
         * \brief The type of the values stored in the map
         * 
         * Needed for Sol2 interaction
         * \see mapType
         */
        using value_type = mapType::value_type;
    
    public:
        /**
         * \brief Constructs a new entity of given type
         * \param manager The entity manager
         * \param name The name of the entity
         */
        explicit Entity(EntityManager &manager, const std::string &name);
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
        mapType::size_type size() const;

        /**
         * \brief Returns a vector with the names of the components this entity has.
         * 
         * The vector is sorted.
         * \return A vector of strings with the names of components this entity has
         */
        std::vector<std::string> getComponentsNames() const;

        /**
         * \brief Returns the name of the entity
         * \return The name of the entity
         */
        const std::string& getName() const;

        /**
         * \brief The begin iterator
         * \return The iterator to the begin of the components' map
         */
        iterator begin();

        /**
         * \brief The end iterator
         * \return The iterator to the end of the components' map
         */
        iterator end();

        /**
         * \brief The const begin iterator
         * \return The const iterator to the begin of the components' map
         */
        const_iterator begin() const;

        /**
         * \brief The const end iterator
         * \return The const iterator to the end of the components' map
         */
        const_iterator end() const;

        /**
         * \brief Registers the Lua functions associated with this class
         * \param lua The Lua state
         */
        static void luaFunctions(sol::state &lua);

    private:
        EntityManager &m_entityManager;
        std::string m_name;
        mapType m_components;

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