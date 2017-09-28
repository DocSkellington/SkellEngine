#pragma once

#include <typeindex>
#include <memory>

#include "entities/components/Component.h"

/**
 * \brief Contains everything related to the entities of a game
 */
namespace engine::entities {
    class Entity final {
    public:
        typedef std::shared_ptr<Entity> Ptr;
    
    public:
        explicit Entity();
        Entity(const Entity&) = delete;
        ~Entity();

    private:
        std::string m_type;
        std::map<std::type_index, std::shared_ptr<components::Component>> m_components;
    };
}