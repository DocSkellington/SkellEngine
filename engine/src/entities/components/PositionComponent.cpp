#include "entities/components/PositionComponent.h"

namespace engine::entities::components {
    Component::RegisterComponent<PositionComponent> PositionComponent::rcpc("position");

    PositionComponent::PositionComponent() :
        Component(),
        m_x(0),
        m_y(0) {
        registerMember("x", &m_x);
        registerMember("int", &m_int);
        std::cout << "before " << m_x << '\n';
        set("x", 5.f);
        std::cout << "after " << m_x << '\n';
        set("int", 5.);
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

    /*
    void PositionComponent::set(const std::string &name, long value) {
        tmx::Logger::log("Position Component: only float values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, double value) {
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
        tmx::Logger::log("Position Component: only float values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, bool value) {
        tmx::Logger::log("Position Component: only float values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, sol::nil_t value) {
        tmx::Logger::log("Position Component: only float values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, const sol::table &value) {
        tmx::Logger::log("Position Component: only integer values are accepted", tmx::Logger::Type::Warning);
    }

    void PositionComponent::set(const std::string &name, nlohmann::json value) {
        tmx::Logger::log("Position Component: only float values are accepted", tmx::Logger::Type::Warning);
    }
    */

    std::pair<long, bool> PositionComponent::getInt(const std::string &name) {
    }

    std::pair<double, bool> PositionComponent::getFloat(const std::string &name) {
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

    std::pair<bool, bool> PositionComponent::getBool(const std::string &name) {
    }

    std::pair<std::string, bool> PositionComponent::getString(const std::string &name) {
    }

    std::pair<sol::object, bool> PositionComponent::getObject(const std::string &name) {
        auto t = getFloat(name);
        return std::make_pair(sol::make_object<int>(*getContext().lua, t.first), t.second);
    }
}
