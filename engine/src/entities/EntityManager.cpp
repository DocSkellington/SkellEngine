#include "SkellEngine/entities/EntityManager.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_lua.h"
#include "SkellEngine/systems/SystemManager.h"

namespace engine::entities {
    EntityManager::EntityManager(states::StateContext &context) :
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

    Entity::Ptr EntityManager::addEntity(const std::string &entityName) {
        Entity::Ptr entity = std::make_shared<Entity>(*this, entityName);
        m_entities.push_back(entity);
        return entity;
    }

    Entity::Ptr EntityManager::addEntity(const std::string &entityName, const nlohmann::json &jsonTable) {
        Entity::Ptr entity = addEntity(entityName);
        
        for (auto itr = jsonTable.begin() ; itr != jsonTable.end() ; ++itr) {
            entity->addComponent(itr.key(), itr.value());
        }

        m_context.systemManager->addEntity(entity);
        return entity;
    }

    Entity::Ptr EntityManager::getEntity(const std::string &name) {
        for (auto &entity : m_entities) {
            if (entity->getName() == name) {
                return entity;
            }
        }
        return nullptr;
    }

    Entity::Ptr EntityManager::getEntity(const std::string &name, std::vector<std::string> components) {
        std::sort(components.begin(), components.end());
        for (auto &entity : m_entities) {
            if (entity->getName() == name && std::includes(entity->getComponentsNames().begin(), entity->getComponentsNames().end(), components.begin(), components.end())) {
                return entity;
            }
        }
        return nullptr;
    }

    std::size_t EntityManager::getNumberOfEntities() const {
        return m_entities.size();
    }

    states::StateContext &EntityManager::getContext() {
        return m_context;
    }

    const states::StateContext &EntityManager::getContext() const {
        return m_context;
    }

    void EntityManager::luaFunctions(sol::state &lua) {
        lua.new_usertype<EntityManager>("EntityManager",
            "addEntity", sol::overload(
                sol::resolve<Entity::Ptr(const std::string&)>(&EntityManager::addEntity),
                sol::resolve<Entity::Ptr(const std::string&, const sol::table&)>(&EntityManager::addEntity)
            ),
            "getEntity", sol::overload(
                sol::resolve<Entity::Ptr(const std::string&)>(&EntityManager::getEntity),
                sol::resolve<Entity::Ptr(const std::string&, const sol::table&)>(&EntityManager::getEntity)
            ),
            "removeEntity", &EntityManager::removeEntity
        );

        lua["game"]["entityManager"] = this;
    }

    Entity::Ptr EntityManager::addEntity(const std::string &name, const sol::table &luaTable) {
        return addEntity(name, utilities::lua_to_json(luaTable));
    }

    Entity::Ptr EntityManager::getEntity(const std::string &name, const sol::table &components) {
        std::vector<std::string> componentsVector(components.size());

        std::size_t i = 0;
        for (auto &compo : components) {
            // We want only unnamed tables
            if (compo.first.get_type() == sol::type::number) {
                // We accept only strings
                if (compo.second.get_type() == sol::type::string) {
                    componentsVector[i++] = compo.second.as<std::string>();
                }
                else {
                    tmx::Logger::log("EntityManager: Invalid use of getEntity(name, components). The 'components' table can only contain string values. The named value " + std::to_string(compo.first.as<int>()) + " will be ignored.", tmx::Logger::Type::Warning);
                }
            }
            else {
                tmx::Logger::log("EntityManager: Invalid use of getEntity(name, components). The 'components' table can only contain unnamed values. The named value " + compo.first.as<std::string>() + " will be ignored.", tmx::Logger::Type::Warning);
            }
        }

        return getEntity(name, componentsVector);
    }
}