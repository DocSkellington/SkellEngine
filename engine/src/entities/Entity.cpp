#include "SkellEngine/entities/Entity.h"

#include <SkellEngine/tmxlite/Log.hpp>
#include "SkellEngine/utilities/json_lua.h"
#include "SkellEngine/entities/components/ExternComponent.h"

namespace engine::entities {
    Entity::Entity(Context &context) :
        m_context(context) {

    }

    Entity::Entity(Context &context, const std::string &type) :
        m_context(context), m_type(type) {

    }

    Entity::~Entity() {

    }

    components::Component::Ptr Entity::addComponent(const std::string &componentType) {
        if (m_components.find(componentType) != m_components.end()) {
            tmx::Logger::log("it is impossible to add two components of the same type (" + componentType + ")", tmx::Logger::Type::Error);
            return nullptr;
        }

        auto compo = components::Component::createInstance(m_context, componentType);
        if (!compo) {
            tmx::Logger::log("Impossible to create a " + componentType, tmx::Logger::Type::Error);
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

    components::Component::Ptr Entity::getComponent(const std::string &componentType) {
        auto itr = m_components.find(componentType);
        if (itr == m_components.end())
            return components::Component::Ptr();
        return itr->second;
    }

    std::size_t Entity::size() const {
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

    const std::string &Entity::getType() const {
        return m_type;
    }

    Entity::iterator Entity::begin() {
        return m_components.begin();
    }

    Entity::iterator Entity::end() {
        return m_components.end();
    }

    Entity::const_iterator Entity::cbegin() const {
        return m_components.cbegin();
    }

    Entity::const_iterator Entity::cend() const {
        return m_components.cend();
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
            "getType", &Entity::getType
        );
    }

    Entity::iterator Entity::begin(lua_State*, Entity& entity) {
        return entity.begin();
    }

    Entity::iterator Entity::end(lua_State*, Entity& entity) {
        return entity.end();
    }
}