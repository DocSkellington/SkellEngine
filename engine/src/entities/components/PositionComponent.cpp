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
        if (name == "x") {
            m_x = value;
        }
        else if (name == "y") {
            m_y = value;
        }
        else {
            tmx::Logger::log("Position Component: " + name + " is not a supported field", tmx::Logger::Type::Warning);
        }
    }

    void PositionComponent::set(const std::string &name, const std::string &value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, bool value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, sol::nil_t value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, const sol::table &value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, nlohmann::json value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    std::pair<int, bool> PositionComponent::getInt(const std::string &name) {
        if (name == "x") {
            return std::make_pair(m_x, true);
        }
        else if (name == "y") {
            return std::make_pair(m_y, true);
        }
        else {
            tmx::Logger::log("Position Component: " + name + " is not a supported field", tmx::Logger::Type::Warning);
            return std::make_pair(0, false);
        }
    }

    std::pair<sol::object, bool> PositionComponent::getObject(const std::string &name) {
        auto t = getInt(name);
        return std::make_pair(sol::make_object<int>(*getContext().lua, t.first), t.second);
    }
}
