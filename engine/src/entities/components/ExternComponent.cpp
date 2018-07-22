#include "entities/components/ExternComponent.h"
#include <sol.hpp>

#include "utilities/json_lua.h"
#include "Context.h"

namespace engine::entities::components {
    ExternComponent::ExternComponent() :
        Component() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(const nlohmann::json &jsonTable) {
        this->m_jsonTable = jsonTable;
    }

    void ExternComponent::set(const std::string &name, int value) {
        std::cout << "LONG\n";
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, const std::string &value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, bool value) {
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

    std::tuple<int, bool> ExternComponent::getInt(const std::string &name) {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            return std::make_tuple(itr->get<int>(), true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_tuple(0, false);
        }
    }

    std::tuple<sol::object, bool> ExternComponent::getObject(const std::string &name) {
        std::cout << name << '\n';
        std::cout << m_jsonTable << '\n';
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            std::cout << *itr << '\n';
            std::cout << itr->type_name() << '\n';
            sol::object o;
            switch(itr->type()) {
            case nlohmann::json::value_t::array:
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
            case nlohmann::json::value_t::object:
                std::cout << "object\n";
                o = sol::make_object<std::string>(*getContext().lua, "OBJECT");
                break;
            default:
                std::cout << "What are you?\n";
                break;
            }
            return std::make_pair(o, true);
        }
        else {
            tmx::Logger::log("ExternComponent: the value " + name + " is undefined.");
            return std::make_tuple(sol::nil, false);
        }
    }
}