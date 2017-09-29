#include "entities/EntityManager.h"

#include "Context.h"

namespace engine::entities {
    EntityManager::EntityManager(Context &context) :
        m_context(context)  {

    }

    void EntityManager::addEntity(const std::string &entityType, const nlohmann::json &jsonTable) {
        Entity::Ptr entity = std::make_shared<Entity>(entityType);
        
        for (auto itr = jsonTable.begin() ; itr != jsonTable.end() ; ++itr) {
            entity->addComponent(itr.key(), itr.value());
        }
    }
}