#include "SkellEngine/entities/components/PositionComponent.hpp"

#include "SkellEngine/errors/InvalidJSON.hpp"

namespace engine::entities::components {
    PositionComponent::PositionComponent(states::StateContext &context) :
        Component(context),
        m_position(0, 0),
        m_absolute(true) {
        registerMember("x", &m_position.x);
        registerMember("y", &m_position.y);
        registerMember("position", &m_position);
        registerMember("absolute", &m_absolute);
    }

    PositionComponent::~PositionComponent() {

    }

    void PositionComponent::create(const nlohmann::json &jsonTable) {
        if (jsonTable.is_array()) {
            if (jsonTable.size() >= 2 && jsonTable.size() <= 3) {
                if (jsonTable[0].is_number() && jsonTable[1].is_number()) {
                    m_position.x = jsonTable[0].get<double>();
                    m_position.y = jsonTable[1].get<double>();

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
                m_position.x = x->get<double>();
            }
            else {
                throw errors::InvalidJSON("Position component: if the JSON is an object, the 'x' field must be present and must be a number");
            }

            if (auto y = jsonTable.find("y") ; y != jsonTable.end() && y->is_number()) {
                m_position.y = y->get<float>();
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
