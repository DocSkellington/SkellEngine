#include "SkellEngine/entities/Entity.hpp"

#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/entities/components/ExternComponent.hpp"
#include "SkellEngine/entities/EntityManager.hpp"

namespace engine::entities {

    Entity::Entity(EntityManager &manager, const std::string &name) :
        m_entityManager(manager),
        m_name(name) {

    }

    Entity::~Entity() {

    }

    components::Component::Ptr Entity::addComponent(const std::string &componentType) {
        if (m_components.find(componentType) != m_components.end()) {
            m_entityManager.getContext().context.logger.log("Entity: Impossible to add two components of the same type (" + componentType + ")", LogType::Error);
            return nullptr;
        }

        auto compo = components::Component::createInstance(m_entityManager.getContext(), componentType);
        if (!compo) {
            m_entityManager.getContext().context.logger.log("Entity: Impossible to create a " + componentType, LogType::Error);
            return nullptr;
        }

        m_components[componentType] = compo;

        return compo;
    }

    components::Component::Ptr Entity::addComponent(const std::string &componentType, const nlohmann::json &jsonTable) {
        auto compo = addComponent(componentType);

        compo->create(jsonTable);
        return compo;
    }

    components::Component::Ptr Entity::addComponent(const std::string &componentType, const sol::table &luaTable) {
        return addComponent(componentType, utilities::lua_to_json(luaTable));
    }

    bool Entity::hasComponent(const std::string &componentType) const {
        return m_components.find(componentType) != m_components.end();
    }

    bool Entity::hasComponents(const std::string &componentType) const {
        return hasComponent(componentType);
    }

    components::Component::Ptr Entity::getComponent(const std::string &componentType) {
        auto itr = m_components.find(componentType);
        if (itr == m_components.end())
            return components::Component::Ptr();
        return itr->second;
    }

    Entity::mapType::size_type Entity::size() const {
        return m_components.size();
    }

    std::vector<std::string> Entity::getComponentsNames() const {
        std::vector<std::string> vec(m_components.size());
        std::size_t i = 0;
        for (auto &c : m_components) {
            vec[i++] = c.first;
        }
        std::sort(vec.begin(), vec.end());
        return vec;
    }

    const std::string &Entity::getName() const {
        return m_name;
    }

    Entity::iterator Entity::begin() {
        return m_components.begin();
    }

    Entity::iterator Entity::end() {
        return m_components.end();
    }

    Entity::const_iterator Entity::begin() const {
        return m_components.begin();
    }

    Entity::const_iterator Entity::end() const {
        return m_components.end();
    }

    void Entity::luaFunctions(sol::state &lua) {
        lua.new_usertype<Entity>("Entity",
            "addComponent", sol::overload(
                sol::resolve<components::Component::Ptr(const std::string&, const sol::table&)>(&Entity::addComponent),
                sol::resolve<components::Component::Ptr(const std::string&)>(&Entity::addComponent)
            ),
            "hasComponent", &Entity::hasComponent,
            "getComponent", &Entity::getComponent,
            "getComponentsNames", &Entity::getComponentsNames,
            "getName", &Entity::getName
        );
    }
}