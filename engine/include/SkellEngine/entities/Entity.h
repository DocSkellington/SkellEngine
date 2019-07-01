#pragma once

#include <typeindex>
#include <memory>

#include "SkellEngine/entities/components/Component.h"

/**
 * \brief Contains everything related to the entities of a game
 */
namespace engine::entities {
    /**
     * \brief An entity is made of zero, one or multiple components.
     * \ingroup Engine
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
        using mapType = std::map<std::string, components::Component::Ptr>;
        /**
         * \brief Allows to iterate over the components
         */
        using iterator = mapType::iterator;
        /**
         * \brief Allows to iterate (with a const iterator) over the components
         */
        using const_iterator = mapType::const_iterator;

        using key_type = mapType::key_type;
        using mapped_type = mapType::mapped_type;

        using value_type = mapType::value_type;
    
    public:
        /**
         * \brief Constructs a new entity of given type
         * \param context The context
         * \param type The type of the entity
         */
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
        mapType::size_type size() const;

        /**
         * \brief Returns a vector with the names of the components this entity has.
         * 
         * The vector is sorted.
         * \return A vector of strings with the names of components this entity has
         */
        std::vector<std::string> getComponentsNames() const;

        /**
         * \brief Returns the type of the entity
         * \return The type of the entity
         */
        const std::string& getType() const;

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
        Context &m_context;
        std::string m_type;
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