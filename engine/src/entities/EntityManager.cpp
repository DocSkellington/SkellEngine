#include "entities/EntityManager.h"

#include "Context.h"

namespace engine::entities {
    EntityManager::EntityManager(Context &context) :
        m_context(context)  {

    }

    bool EntityManager::removeEntity(Entity::Ptr entity) {
        m_context.systemManager->removeEntity(entity);

        bool deleted = false;
        for (auto itr = m_entities.begin() ; itr != m_entities.end() ; ++itr) {
            if (*itr == entity) {
                m_entities.erase(itr);
                deleted = true;
            }
        }
        return deleted;
    }

    Entity::Ptr EntityManager::addEntity(const std::string &entityType) {
        Entity::Ptr entity = std::make_shared<Entity>(m_context, entityType);
        return entity;
    }

    Entity::Ptr EntityManager::addEntity(const std::string &entityType, const nlohmann::json &jsonTable) {
        Entity::Ptr entity = std::make_shared<Entity>(m_context, entityType);
        
        for (auto itr = jsonTable.begin() ; itr != jsonTable.end() ; ++itr) {
            entity->addComponent(itr.key(), itr.value());
        }

        m_context.systemManager->addEntity(entity);
        return entity;
    }

    void EntityManager::luaFunctions(sol::state &lua) {
        lua.new_usertype<EntityManager>("EntityManager",
            "addEntity", sol::resolve<Entity::Ptr(const std::string&)>(&EntityManager::addEntity)
        );

        lua["entMan"] = this;
    }
}