#include "SkellEngine/entities/components/ExternComponent.h"
#include <sol/sol.hpp>

#include "SkellEngine/utilities/json_lua.h"
#include "SkellEngine/Context.h"

namespace engine::entities::components {
    ExternComponent::ExternComponent() :
        Component() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(const nlohmann::json &jsonTable) {
        this->m_jsonTable = jsonTable;
    }

    /*
    void ExternComponent::set(const std::string &name, long value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, double value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, bool value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, const std::string &value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, sol::nil_t value) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            m_jsonTable.erase(itr);
        }
    }

    void ExternComponent::set(const std::string &name, const sol::table &value) {
        set(name, utilities::lua_to_json(value));
    }

    void ExternComponent::set(const std::string &name, nlohmann::json value) {
        m_jsonTable[name] = value;
    }
    */

    std::pair<long, bool> ExternComponent::getInt(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            return std::make_pair(itr->get<long>(), true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_pair(0, false);
        }
    }

    std::pair<double, bool> ExternComponent::getFloat(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            return std::make_pair(itr->get<double>(), true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_pair(0.f, false);
        }
    }

    std::pair<bool, bool> ExternComponent::getBool(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            return std::make_pair(itr->get<bool>(), true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_pair(false, false);
        }
    }

    std::pair<std::string, bool> ExternComponent::getString(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            return std::make_pair(itr->get<std::string>(), true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_pair("", false);
        }
    }

    std::pair<sol::object, bool> ExternComponent::getObject(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            sol::object o;
            switch(itr->type()) {
            case nlohmann::json::value_t::array:
            case nlohmann::json::value_t::object:
                o = utilities::json_to_lua(*itr, *(getContext().lua));
                break;
            case nlohmann::json::value_t::number_float:
                o = sol::make_object<double>(*getContext().lua, itr->get<double>());
                break;
            case nlohmann::json::value_t::number_integer:
                o = sol::make_object<long>(*getContext().lua, itr->get<long>());
                break;
            case nlohmann::json::value_t::number_unsigned:
                o = sol::make_object<unsigned long>(*getContext().lua, itr->get<unsigned long>());
                break;
            case nlohmann::json::value_t::boolean:
                o = sol::make_object<bool>(*getContext().lua, itr->get<bool>());
                break;
            case nlohmann::json::value_t::string:
                o = sol::make_object<std::string>(*getContext().lua, itr->get<std::string>());
                break;
            case nlohmann::json::value_t::null:
                // We create an empty table
                o = getContext().lua->create_table();
                break;
            default:
                // TODO:
                std::cout << "What are you?\n";
                break;
            }
            return std::make_pair(o, true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.", tmx::Logger::Type::Warning);
            return std::make_pair(sol::nil, false);
        }
    }
}