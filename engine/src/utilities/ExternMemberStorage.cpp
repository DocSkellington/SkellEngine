#include "SkellEngine/utilities/ExternMemberStorage.h"

#include "SkellEngine/utilities/json_lua.h"
#include "SkellEngine/Context.h"

namespace engine::utilities {
    ExternMemberStorage::ExternMemberStorage(Context &context) :
        MemberStorage(context) {

    }

    void ExternMemberStorage::set(const std::string &name, int value) noexcept {
        setInternally<int>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, long value) noexcept {
        setInternally<long>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, float value) noexcept {
        setInternally<float>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, double value) noexcept {
        setInternally<double>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, bool value) noexcept {
        setInternally<bool>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, const char* value) noexcept {
        setInternally<const char*>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, const std::string &value) noexcept {
        setInternally<const std::string&>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, const nlohmann::json &value) noexcept {
        setInternally<const nlohmann::json&>(name, value);
    }

    void ExternMemberStorage::set(const std::string &name, sol::nil_t) noexcept {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            m_jsonTable.erase(itr);
        }
    }

    void ExternMemberStorage::set(const std::string &name, const sol::table &value) noexcept {
        setInternally<const nlohmann::json&>(name, utilities::lua_to_json(value));
    }

    bool ExternMemberStorage::has(const std::string &name) const noexcept {
        return m_jsonTable.find(name) != m_jsonTable.end();
    }

    std::pair<long, bool> ExternMemberStorage::getInt(const std::string &name) const {
        return getValue(name, 0L);
    }

    std::pair<double, bool> ExternMemberStorage::getFloat(const std::string &name) const {
        return getValue(name, 0.);
    }

    std::pair<bool, bool> ExternMemberStorage::getBool(const std::string &name) const {
        return getValue(name, false);
    }

    std::pair<std::string, bool> ExternMemberStorage::getString(const std::string &name) const {
        return getValue<std::string>(name, "");
    }

    std::pair<nlohmann::json, bool> ExternMemberStorage::getJSON(const std::string &name) const {
        return getValue<nlohmann::json>(name, nlohmann::json());
    }

    std::pair<sol::object, bool> ExternMemberStorage::getObject(const std::string &name) const {
        auto itr = m_jsonTable.find(name);
        if (itr != m_jsonTable.end()) {
            sol::object o(sol::nil);
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
                getContext().logger.log(getLogErrorPrefix() + ": unknown value type. nil will be returned");
                break;
            }
            return std::make_pair(o, true);
        }
        else {
            return std::make_pair(sol::nil, false);
        }
    }

    void ExternMemberStorage::setInternalJSONTable(const nlohmann::json &jsonTable) {
        m_jsonTable = jsonTable;
    }

    nlohmann::json &ExternMemberStorage::getInternalJSONTable() {
        return m_jsonTable;
    }

    const nlohmann::json &ExternMemberStorage::getInternalJSONTable() const {
        return m_jsonTable;
    }

    void ExternMemberStorage::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternMemberStorage>("ExternEvent",
            "get", &ExternMemberStorage::getObject,
            "set", sol::overload(
                sol::resolve<void(const std::string&, long)>(&ExternMemberStorage::set),
                sol::resolve<void(const std::string&, double)>(&ExternMemberStorage::set),
                sol::resolve<void(const std::string&, bool)>(&ExternMemberStorage::set),
                sol::resolve<void(const std::string&, const std::string&)>(&ExternMemberStorage::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&ExternMemberStorage::set),
                sol::resolve<void(const std::string&, const sol::table&)>(&ExternMemberStorage::set)
            ),
            "has", &ExternMemberStorage::has,
            sol::base_classes, sol::bases<utilities::MemberStorage>()
        );
    }
}