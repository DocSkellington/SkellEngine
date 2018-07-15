#include "entities/components/ExternComponent.h"
#include <sol.hpp>

namespace engine::entities::components {
    ExternComponent::ExternComponent() : Component() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(Context &context, const nlohmann::json &jsonTable) {
        this->m_jsonTable = jsonTable;
    }

    void ExternComponent::set(const std::string &name, long value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, unsigned long value) {
        m_jsonTable[name] = value;
    }

    void ExternComponent::set(const std::string &name, std::string value) {
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

    void ExternComponent::set(const std::string &name, sol::table value) {
        
    }

    void ExternComponent::set(const std::string &name, nlohmann::json value) {
        m_jsonTable[name] = value;
    }

    int ExternComponent::get(const std::string &name) {
        return 5;
    }

    void ExternComponent::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternComponent>("component",
            sol::constructors<ExternComponent()>(),

            "getValue", &ExternComponent::get,

            "setValue", sol::overload(
                sol::resolve<void(const std::string&, long)>(&ExternComponent::set),
                sol::resolve<void(const std::string&, unsigned long)>(&ExternComponent::set),
                sol::resolve<void(const std::string&, std::string)>(&ExternComponent::set),
                sol::resolve<void(const std::string&, bool)>(&ExternComponent::set),
                sol::resolve<void(const std::string&, sol::table)>(&ExternComponent::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&ExternComponent::set)
            )
        );
    }
}