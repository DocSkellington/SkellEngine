#include "SkellEngine/utilities/VariableStorage.hpp"

#include "SkellEngine/Context.hpp"

namespace engine::utilities {
    VariableStorage::VariableStorage(Context &context) :
        m_context(context) {

    }

    VariableStorage::~VariableStorage() {

    }

    bool VariableStorage::has(const std::string &name) const noexcept {
        if (auto itr = getIterator(name); itr != mapMembers.end()) {
            return itr->second.has_value();
        }
        return false;
    }

    void VariableStorage::loadFromJSON(const nlohmann::json &json) {
        for (auto &[key, value] : json.items()) {
            if (value.is_boolean()) {
                set(key, value.get<bool>());
            }
            else if (value.is_number_float()) {
                set(key, value.get<double>());
            }
            else if (value.is_number_integer()) {
                set(key, value.get<long>());
            }
            else if (value.is_number_unsigned()) {
                set(key, value.get<unsigned long>());
            }
            else if (value.is_string()) {
                set(key, value.get<std::string>());
            }
            else {
                // If value's type is not trivial, we store the JSON as is
                set(key, value);
            }
        }
    }

    void VariableStorage::luaFunctions(sol::state &lua) {
        lua.new_usertype<VariableStorage>("VariableStorage",
            "get", &VariableStorage::getForLua,

            "set", sol::overload(
                static_cast<void (VariableStorage::*)(const std::string&, double)>(&VariableStorage::set),
                static_cast<void (VariableStorage::*)(const std::string&, long)>(&VariableStorage::set),
                static_cast<void (VariableStorage::*)(const std::string&, bool)>(&VariableStorage::set),
                static_cast<void (VariableStorage::*)(const std::string&, const std::string&)>(&VariableStorage::set),
                static_cast<void (VariableStorage::*)(const std::string&, sol::nil_t)>(&VariableStorage::set),
                static_cast<void (VariableStorage::*)(const std::string&, const sol::table&)>(&VariableStorage::set)
            ),

            "has", &VariableStorage::has
        );
    }

    void VariableStorage::logUnknownVariable(const std::string &name) const {
        m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known variable", LogType::Warning);
    }

    VariableStorage::Container::const_iterator VariableStorage::getIterator(const std::string &name) const {
        if (auto itr = mapMembers.find(name); itr != mapMembers.end()) {
            return itr;
        }
        else if (auto itr = mapFree.find(name); itr != mapFree.end()) {
            return itr;
        }
        else {
            logUnknownVariable(name);
            return mapMembers.end();
        }
    }

    Context &VariableStorage::getContext() {
        return m_context;
    }

    const Context &VariableStorage::getContext() const {
        return m_context;
    }

    std::pair<sol::object, bool> VariableStorage::getForLua(const std::string &name, const sol::object &defaultValue) const {
        return get<sol::object>(name, defaultValue);
    }
}