#include "SkellEngine/entities/components/PositionComponent.h"

namespace engine::entities::components {
    PositionComponent::PositionComponent(states::StateContext &context) :
        MemberStorage(context.context),
        Component(context),
        m_x(0),
        m_y(0) {
        registerMember("x", &m_x);
        registerMember("y", &m_y);
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(const nlohmann::json &jsonTable) {
        if (jsonTable.type() == nlohmann::json::value_t::array) {
            m_x = jsonTable[0].get<float>();
            m_y = jsonTable[1].get<float>();
        }
        else if (jsonTable.type() == nlohmann::json::value_t::object) {
            m_x = jsonTable["x"].get<float>();
            m_y = jsonTable["y"].get<float>();
        }
    }
}
