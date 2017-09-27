#pragma once

#include <vector>

#include "Entity.h"

namespace engine::entities {
    struct Context;

    class EntityManager final {
    public:
        explicit EntityManager(Context &context);
        EntityManager(const EntityManager&) = delete;

        void addEntity(const std::string &type);

    private:
        std::vector<Entity::Ptr> m_entities;
        Context &m_context;
    };
}