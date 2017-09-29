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
        auto compo = components::Component::createInstance(componentType);
        if (!compo) {
            std::cerr << "Error: Impossible to create a " << componentType << '\n';
            return;
        }

        compo->create(jsonTable);
        m_components[typeid(componentType)] = compo;
    }
}