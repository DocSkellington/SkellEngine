#include "entities/EntityManager.h"

#include "Context.h"

namespace engine::entities {
    EntityManager::EntityManager(Context &context) :
        m_context(context)  {

    }

    bool EntityManager::removeEntity(Entity::Ptr entity) {
        m_context.systemManager->removeEntity(entity);

        for (auto itr = m_entities.begin() ; itr != m_entities.end() ; ++itr) {
            if (*itr == entity) {
                m_entities.erase(itr);
                return true;
            }
        }
        return false;
    }

    Entity::Ptr EntityManager::addEntity(const std::string &entityType) {
        Entity::Ptr entity = std::make_shared<Entity>(m_context, entityType);
        m_entities.push_back(entity);
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

    Entity::Ptr EntityManager::getEntity(const std::string &type) {
        for (auto &entity : m_entities) {
            if (entity->getType() == type) {
                return entity;
            }
        }
        return nullptr;
    }

    Entity::Ptr EntityManager::getEntity(const std::string &type, std::vector<std::string> components) {
        std::sort(components.begin(), components.end());
        for (auto &entity : m_entities) {
            if (entity->getType() == type && std::includes(entity->getComponentsNames().begin(), entity->getComponentsNames().end(), components.begin(), components.end())) {
                return entity;
            }
        }
        return nullptr;
    }

    std::size_t EntityManager::getNumberOfEntities() const {
        return m_entities.size();
    }

    void EntityManager::luaFunctions(sol::state &lua) {
        lua.new_usertype<EntityManager>("EntityManager",
            "addEntity", sol::resolve<Entity::Ptr(const std::string&)>(&EntityManager::addEntity)
        );

        lua["entMan"] = this;
    }
}