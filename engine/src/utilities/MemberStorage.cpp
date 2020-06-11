#include "SkellEngine/utilities/MemberStorage.hpp"

#include "SkellEngine/utilities/json_lua.hpp"
#include "SkellEngine/Context.hpp"

namespace engine::utilities {
    MemberStorage::MemberStorage(Context &context) :
        m_context(context) {

    }

    MemberStorage::~MemberStorage() {

    }

    void MemberStorage::set(const std::string &name, int value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            // We can store a int in: int, long, float, double
            if (itr->second.type() == typeid(int*)) {
                convertAndSet<int>(itr->second, value);
            }
            else if (itr->second.type() == typeid(long*)) {
                convertAndSet<long>(itr->second, value);
            }
            else if (itr->second.type() == typeid(float*)) {
                convertAndSet<float>(itr->second, value);
            }
            else if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with int", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, long value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            // We can store a long in: long, double
            if (itr->second.type() == typeid(long*)) {
                convertAndSet<long>(itr->second, value);
            }
            else if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with long", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, float value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            // We can store a float in: float, double
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else if (itr->second.type() == typeid(float*)) {
                convertAndSet<float>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with float", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, double value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with double", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, bool value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                convertAndSet<bool>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with bool", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, const char* value) noexcept {
        set(name, std::string(value));
    }

    void MemberStorage::set(const std::string &name, const std::string& value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                convertAndSet<std::string>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with std::string", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    void MemberStorage::set(const std::string &name, sol::nil_t) noexcept {
        // We set the variable at the default value
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                convertAndSet<int>(itr->second, 0);
            }
            else if (itr->second.type() == typeid(long*)) {
                convertAndSet<long>(itr->second, 0L);
            }
            else if (itr->second.type() == typeid(float*)) {
                convertAndSet<float>(itr->second, 0.f);
            }
            else if (itr->second.type() == typeid(double*)) {
                convertAndSet<double>(itr->second, 0.);
            }
            else if (itr->second.type() == typeid(bool*)) {
                convertAndSet<bool>(itr->second, false);
            }
            else if (itr->second.type() == typeid(std::string*)) {
                convertAndSet<std::string>(itr->second, "");
            }
            else if (itr->second.type() == typeid(nlohmann::json*)) {
                convertAndSet<nlohmann::json>(itr->second, nlohmann::json());
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with sol::nil_t", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }

    }

    void MemberStorage::set(const std::string &name, const sol::table& value) noexcept {
        set(name, utilities::lua_to_json(value));
    }

    void MemberStorage::set(const std::string &name, const nlohmann::json &value) noexcept {
        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                convertAndSet<nlohmann::json>(itr->second, value);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": type of " + name + " (" + itr->second.type().name() + ") is incompatible with nlohmann::json", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": set " + name + ": " + name + " is not a known member", LogType::Warning);
        }
    }

    bool MemberStorage::has(const std::string &name) const noexcept {
        if (auto itr = mapMembers.find(name) ; itr != mapMembers.end()) {
            return itr->second.has_value();
        }
        return false;
    }

    std::pair<long, bool> MemberStorage::getInt(const std::string &name) const {
        std::pair<long, bool> pair = std::make_pair(0, false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(getValue<int>(itr->second), true);
            }
            else if (itr->second.type() == typeid(long*))  {
                pair = std::make_pair(getValue<long>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not an integer (int or long) variable", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }

    std::pair<double, bool> MemberStorage::getFloat(const std::string &name) const {
        std::pair<double, bool> pair = std::make_pair(0., false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(getValue<float>(itr->second), true);
            }
            else if (itr->second.type() == typeid(double*))  {
                pair = std::make_pair(getValue<double>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a floating point (float or double) variable", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }
     
    std::pair<bool, bool> MemberStorage::getBool(const std::string &name) const {
        std::pair<bool, bool> pair = std::make_pair(false, false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(getValue<bool>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a boolean variable", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }
     
    std::pair<std::string, bool> MemberStorage::getString(const std::string &name) const {
        std::pair<std::string, bool> pair = std::make_pair("", false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(getValue<std::string>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a std::string variable", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }

    std::pair<nlohmann::json, bool> MemberStorage::getJSON(const std::string &name) const {
        std::pair<nlohmann::json, bool> pair = std::make_pair(nlohmann::json(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(getValue<nlohmann::json>(itr->second), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a JSON variable", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": get " + name + ": " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }
     
    std::pair<sol::object, bool> MemberStorage::getObject(const std::string &name) const {
        std::pair<sol::object, bool> pair = std::make_pair(sol::nil_t(), false);

        auto itr = mapMembers.find(name);
        if (itr != mapMembers.end()) {
            if (itr->second.type() == typeid(int*)) {
                pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<int>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(long*)) {
                pair = std::make_pair(sol::make_object<long>(*getContext().lua, getValue<long>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(float*)) {
                pair = std::make_pair(sol::make_object<double>(*getContext().lua, getValue<float>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(double*)) {
                pair = std::make_pair(sol::make_object<double>(*getContext().lua, getValue<double>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(bool*)) {
                pair = std::make_pair(sol::make_object<bool>(*getContext().lua, getValue<bool>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(std::string*)) {
                pair = std::make_pair(sol::make_object<std::string>(*getContext().lua, getValue<std::string>(itr->second)), true);
            }
            else if (itr->second.type() == typeid(nlohmann::json*)) {
                pair = std::make_pair(sol::make_object<nlohmann::json>(*getContext().lua, getValue<nlohmann::json>(itr->second)), true);
            }
            else {
                m_context.logger.log(getLogErrorPrefix() + ": getObject: " + name + " does not have an appropriate type", LogType::Warning);
            }
        }
        else {
            m_context.logger.log(getLogErrorPrefix() + ": getObject: " + name + " is not a known member", LogType::Warning);
        }

        return pair;
    }

    void MemberStorage::luaFunctions(sol::state &lua) {
        lua.new_usertype<MemberStorage>("memberStorage",
            "get", &MemberStorage::getObject,

            "set", sol::overload(
                sol::resolve<void(const std::string&, double)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, long)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, bool)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, const std::string&)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, sol::nil_t)>(&MemberStorage::set),
                sol::resolve<void(const std::string&, const sol::table&)>(&MemberStorage::set)
            ),

            "has", &MemberStorage::has
        );
    }

    Context &MemberStorage::getContext() {
        return m_context;
    }

    const Context &MemberStorage::getContext() const {
        return m_context;
    }

    MemberStorage::Container& MemberStorage::getContainer() {
        return mapMembers;
    }

    const MemberStorage::Container& MemberStorage::getContainer() const {
        return mapMembers;
    }
}