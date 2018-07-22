#include "entities/EntityManager.h"

#include "Context.h"

namespace engine::entities {
    EntityManager::EntityManager(Context &context) :
        m_context(context)  {

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