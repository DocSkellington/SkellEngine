#include "entities/Entity.h"

namespace engine::entities {
    Entity::Entity() {

    }

    Entity::Entity(const std::string &type) :
        m_type(type) {

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

        compo->create(jsonTable);
        m_components[componentType] = compo;
    }
}