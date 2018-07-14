#include "entities/components/PositionComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<PositionComponent> PositionComponent::rcpc("position");

    PositionComponent::PositionComponent() :
        Component() {
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(Context &context, const nlohmann::json &jsonTable) {
        m_x = jsonTable[0].get<float>();
        m_y = jsonTable[1].get<float>();
    }
}
