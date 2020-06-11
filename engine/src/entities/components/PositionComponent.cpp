#include "SkellEngine/entities/components/PositionComponent.hpp"

#include "SkellEngine/errors/InvalidJSON.hpp"

namespace engine::entities::components {
    PositionComponent::PositionComponent(states::StateContext &context) :
        MemberStorage(context.context),
        Component(context),
        m_x(0),
        m_y(0),
        m_absolute(true) {
        registerMember("x", &m_x);
        registerMember("y", &m_y);
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(const nlohmann::json &jsonTable) {
        if (jsonTable.is_array()) {
            if (jsonTable.size() >= 2 && jsonTable.size() <= 3) {
                if (jsonTable[0].is_number() && jsonTable[1].is_number()) {
                    m_x = jsonTable[0].get<float>();
                    m_y = jsonTable[1].get<float>();

                    if (jsonTable.size() == 3) {
                        if (jsonTable[2].is_boolean()) {
                            m_absolute = jsonTable[2].get<bool>();
                        }
                        else {
                            throw errors::InvalidJSON("Position component: if the JSON is an array, the third value must be a boolean");
                        }
                    }
                }
                else {
                    throw errors::InvalidJSON("Position component: if the JSON is an array, the first two values must be a number");
                }
            }
            else {
                throw errors::InvalidJSON("Position component: if the JSON is an array, it must contain at least two and at most three values");
            }
        }
        else {
            if (auto x = jsonTable.find("x") ; x != jsonTable.end() && x->is_number()) {
                m_x = x->get<float>();
            }
            else {
                throw errors::InvalidJSON("Position component: if the JSON is an object, the 'x' field must be present and must be a number");
            }

            if (auto y = jsonTable.find("y") ; y != jsonTable.end() && y->is_number()) {
                m_y = y->get<float>();
            }
            else {
                throw errors::InvalidJSON("Position component: if the JSON is an object, the 'y' field must be present and must be a number");
            }

            if (auto absolute = jsonTable.find("absolute") ; absolute != jsonTable.end()) {
                if (absolute->is_boolean()) {
                    m_absolute = absolute->get<bool>();
                }
                else {
                    throw errors::InvalidJSON("Position component: if the JSON is an object, the 'absolute' field must be a boolean");
                }
            }
        }
    }
}
