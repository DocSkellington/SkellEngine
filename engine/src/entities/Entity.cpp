#include "entities/Entity.h"

namespace engine::entities {
    Entity::Entity(Context &context) :
        m_context(context) {

    }

    Entity::Entity(Context &context, const std::string &type) :
        m_context(context), m_type(type) {

    }

    Entity::~Entity() {

    }

    void Entity::addComponent(const std::string &componentType, const nlohmann::json &jsonTable) {
        if (m_components.find(componentType) != m_components.end()) {
            std::cerr << "Error: it is impossible to add two components of the same type (" << componentType << ")\n";
            return;
        }

        auto compo = components::Component::createInstance(componentType);
        if (!compo) {
            std::cerr << "Error: Impossible to create a " << componentType << '\n';
            return;
        }

        compo->create(m_context, jsonTable);
        m_components[componentType] = compo;
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
}