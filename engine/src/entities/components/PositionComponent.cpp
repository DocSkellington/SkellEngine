#include "entities/components/PositionComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<PositionComponent> PositionComponent::rcpc("position");

    PositionComponent::PositionComponent() {
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(const sol::table &luaTable) {

    }

    void PositionComponent::create(const nlohmann::json &jsonTable) {
        m_x = jsonTable[0].get<float>();
        m_y = jsonTable[1].get<float>();
    }

    void PositionComponent::create(const nlohmann::json &jsonTable, const sol::table &luaTable) {

    }
}
    