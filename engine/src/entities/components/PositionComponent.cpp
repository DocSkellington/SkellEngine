#include "entities/components/PositionComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<PositionComponent> PositionComponent::rcpc("position");

    PositionComponent::PositionComponent() :
        Component() {
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(const nlohmann::json &jsonTable) {
        m_x = jsonTable[0].get<float>();
        m_y = jsonTable[1].get<float>();
    }

    void PositionComponent::set(const std::string &name, int value) {

    }

    void PositionComponent::set(const std::string &name, const std::string &value) {

    }

    void PositionComponent::set(const std::string &name, bool value) {

    }

    void PositionComponent::set(const std::string &name, sol::nil_t value) {

    }

    void PositionComponent::set(const std::string &name, const sol::table &value) {

    }

    void PositionComponent::set(const std::string &name, nlohmann::json value) {

    }

    std::tuple<int, bool> PositionComponent::getInt(const std::string &name) {

    }

    std::tuple<sol::object, bool> PositionComponent::getObject(const std::string &name) {

    }
}
